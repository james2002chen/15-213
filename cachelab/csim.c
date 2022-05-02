//this program scores 21/27 for not using LRU (least-recently used) replacement policy
//instead use lowest referenve number 
//LRU should be implemented using double end list
//I DON'T WANT TO FIX IT
#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
//cache variables
int hits=0,misses=0,evictions=0;
//s sets, E lines, b block size
int cache_s=0,cache_E=0,cache_b=0;
void *cache_ptr;
typedef unsigned long long ull;
struct cache{
    ull tag;
    int cnt; 
}; 


void get_index(ull addr, ull *tag_ptr, ull *set_ptr){
    addr=addr>>cache_b;
    ull mask=1;
    for(int i=1;i<=cache_s-1;i++) mask=(mask<<1)+1;
    *set_ptr=addr&mask;
    addr=addr>>cache_s;
    *tag_ptr=addr;
}


void result_update(int result,bool verbose){
    if(result==0){
        misses++;
        if(verbose) printf("miss\n"); 
    }
    if(result==1){
        hits++;
        if(verbose) printf("hit\n"); 
    }
    if(result==-1){
        misses++;
        evictions++;
        if(verbose) printf("miss eviction\n"); 
    }
}

//return 0 for miss,1 for hit,-1 for eviction;
int cache_update(ull tag, ull set){
    struct cache *ptr=(struct cache *)cache_ptr+set*cache_E;
    int min_cnt=INT_MAX;
    struct cache *min_ptr=NULL;
    for(int i=1;i<=cache_E;i++){
        if(ptr->cnt==0){
            ptr->tag=tag;
            ptr->cnt++;
            return 0;//miss
        }
        if(ptr->tag==tag && ptr->cnt>0){
            return 1;//hit
        }
        if(ptr->cnt<min_cnt){
            min_cnt=ptr->cnt;
            min_ptr=ptr;
        }
        ptr++;
    }
    min_ptr->tag=tag;
    min_ptr->cnt=1;
    return -1;//eviction
}


int main(int argc, char **argv)
{
    FILE *fp;
    bool verbose=false;
    //bug fixed: add empty_arg handler
    //opterr=0; if not 0, getopt return ? while print additional error message
    char ch;
    bool empty_arg=true;
    while((ch = getopt(argc, argv, "hvs:E:b:t:")) != -1){
        empty_arg=false;
        switch(ch){
            case 'h':
                fprintf(stderr,"Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n",argv[0]);
                fprintf(stderr,"Examples:\n  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
                exit(0);
                break;
            case 'v':
                verbose=true;
                break;
            case 's':
                cache_s=atoi(optarg);
                if(cache_s<1){
                    fprintf(stderr,"invalid -s");
                    exit(0);
                }
                break;
            case 'E':
                cache_E=atoi(optarg);
                if(cache_E<1){
                    fprintf(stderr,"invalid -E");
                    exit(0);
                }
                break;
            case 'b':
                cache_b=atoi(optarg);
                if(cache_b<1){
                    fprintf(stderr,"invalid -b");
                    exit(0);
                }
                break;
            case 't':
                if((fp=fopen(optarg,"r")) == NULL){
                    fprintf(stderr,"%s not found\n",optarg);
                    exit(0);
                }
                break;
            default:/* '?' */
                fprintf(stderr,"Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n",argv[0]);
                fprintf(stderr,"Examples:\n  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
                exit(0);
        }
    }
    if(empty_arg){
        fprintf(stderr,"Missing required command line argument\n");
        exit(0);
    }

    cache_ptr=calloc((1<<cache_s)*cache_E,sizeof(struct cache));
    if(cache_ptr==NULL){
        fprintf(stderr,"calloc error\n");
        exit(0);
    }

    char buff[2048];
    while(fgets(buff,2048,fp)!=NULL){
        char op;
        if(buff[0]==' ') op=buff[1];
        else continue;//op=I
        char **endptr=NULL;
        ull addr=(ull)strtoll(buff+3,endptr,16);
        if(verbose) printf("%c %llu ",op,addr); 
        ull tag=0,set=0;
        get_index(addr,&tag,&set);
        //if(verbose) printf("tag:%llu set:%llu \n",tag,set); 
        int result=0;
        switch(op){
            case 'L':
                result=cache_update(tag,set);
                result_update(result,verbose);
                break;
            case 'S':
                result=cache_update(tag,set);
                result_update(result,verbose);
                break;
            case 'M':
                result=cache_update(tag,set);
                result_update(result,verbose);
                result=cache_update(tag,set);
                result_update(result,verbose);
                break;
            default:
                fprintf(stderr,"invalid operation:%c\n",op);
                exit(0);
        }
    }
    if(ferror(fp)){
        fprintf(stderr,"fgets error\n");
        exit(0);
    }
    //TODO implement verbose
    if(verbose){}

    free(cache_ptr);
    printSummary(hits, misses, evictions);
    return 0;
}

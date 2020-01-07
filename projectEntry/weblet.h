#ifndef WEBLET_H_INCLUDED
#define WEBLET_H_INCLUDED


#include "../tools/wrapper.h"
void process_trans(int fd);
void read_requesthdrs(rio_t *rp);
int is_static(char *uri);
void parse_static_uri(char *uri,char *filename);
void parse_dynamic_uri(char *uri,char *filename,char *cgiargs);
void feed_static(int fd,char *filename,int filesize);
void get_filetype(char *filename,char *filetype);
void feed_dynamic(int fd,char *filename,char *cgiargs);
void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *description);
void deal_parm_post(char *arg, char *cgiargs);
void split_str_post(char *src,const char *separator,char **dest,int *num);

#endif

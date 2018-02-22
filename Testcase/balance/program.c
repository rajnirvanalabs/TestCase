#include "stdio.h"
#include <stdlib.h>
#include "string.h"
#include <curl/curl.h>



struct recievedata {
  char *ptr;
  size_t len;
};

void init_string(struct recievedata *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}



size_t writefunc(void *ptr, size_t size, size_t nmemb, struct recievedata *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}



int main(int argc, char** argv)

{

 if(argc == 4) { // if there are too few arguments
        printf("ERROR: Expected atleast 4 argument\n");
        return 0;
    }


      char* str1;
      char* str2;
      char* str3;
      char* str4;
      char* str5;
      str1 = "{\"userpass\":\"";
      str2 = "\",\"method\":\"";
      str3 = "\",\"coin\":\"";
      str4 = "\",\"address\":\"";
      str5 = "\"}";

      int v= 1 + strlen(str1)+ strlen(argv[1]) + strlen(str2) + strlen(argv[2]) + strlen(str3) + strlen(argv[3]) + strlen(str4) + strlen(argv[4]) + strlen(str5);
      char * data = (char *) malloc(v);

	strcpy(data, str1);
	strcat(data, argv[1]);
  strcat(data, str2);
	strcat(data, argv[2]);
  strcat(data, str3);
	strcat(data, argv[3]);
  strcat(data, str4);
	strcat(data, argv[4]);
	strcat(data, str5);

  CURL *curl;
  CURLcode res;


  curl_global_init(CURL_GLOBAL_ALL);



  curl = curl_easy_init();
  if(curl) {




    struct recievedata s;
    init_string(&s);


   curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:7783");
   curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

    res = curl_easy_perform(curl);

    if(res != CURLE_OK)
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
    curl_easy_strerror(res));


 printf("%s\n", s.ptr);
 free(s.ptr);



    curl_easy_cleanup(curl);
}
  curl_global_cleanup();
  return 0;
}

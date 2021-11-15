//------------------------------------------------------
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//------------------------------------------------------
void write(char *str);
char *replace(char *str, char c, char rep);
void html_open();
void html_close();
void head_open();
void head_close();
void title(char *str);
void random_list();
void menu();
void body(char *name);
void page(char *name);

//------------------------------------------------------
FILE *fp;

//------------------------------------------------------
void write(char *str) { fputs(str, fp); }

#define REPLACE_BUFFER_MAX 4096
char *replace(char *str, char c, char rep) {
  size_t str_sz = strlen(str);

  if (str_sz > REPLACE_BUFFER_MAX)
    return str;

  static char buf[REPLACE_BUFFER_MAX];
  memset(buf, 0, REPLACE_BUFFER_MAX);

  strncpy(buf, str, str_sz);

  for (int i = 0; i < str_sz; ++i)
    if (buf[i] == c)
      buf[i] = rep;

  return buf;
}

//------------------------------------------------------
void html_open() {
  write("<!DOCTYPE html>"
        "<html>");
}
void html_close() { write("</html>"); }

//------------------------------------------------------
char *theme = "one";

void head_open() {
  write("<head>"
        "<link rel='stylesheet' href='themes/");
  write(theme);
  write(".css' />"
        "<link rel='stylesheet' href='stylesheet.css' />");
}
void head_close() { write("</head>"); }

//------------------------------------------------------
void title(char *str) {
  write("<title>pat: ");
  write(str);
  write("</title>");
  write("<h1>");
  write(str);
  write("</h1>");
}

//------------------------------------------------------
#define NUM_LIST_TYPES 34

// taken from https://developer.mozilla.org/en-US/docs/Web/CSS/list-style-type
char *list_types[NUM_LIST_TYPES] = {
    "decimal",
    "cjk-decimal",
    "lower-roman",
    "lower-greek",
    "lower-alpha",
    "arabic-indic",
    "lower-armenian",
    "bengali",
    "khmer",
    "cjk-earthly-branch",
    "cjk-heavenly-stem",
    "cjk-ideographic",
    "devanagari",
    "ethiopic-numeric",
    "georgian",
    "gujarati",
    "gurmukhi",
    "hebrew",
    "hiragana",
    "japanese-informal",
    "kannada",
    "katakana",
    "korean-hanja-informal",
    "lao",
    "malayalam",
    "mongolian",
    "myanmar",
    "oriya",
    "persian",
    "simp-chinese-informal",
    "tamil",
    "telugu",
    "thai",
    "tibetan",
};

void random_list() {
  write("<ol style='list-style-type: ");
  write(list_types[rand() % NUM_LIST_TYPES]);
  write("'>");
}

//------------------------------------------------------
void menu() {
  random_list();
  write("<li><a href='pat_lives_here.html'>home</a></li>"
        "<li><a href='hmm.html'>hmm</a></li>"
        "<li><a href='vm.html'>code</a></li>"
        "<li><a href='www.google.com'>art</a></li>"
        "<li><a href='www.google.com'>sounds</a></li>"
        "<li><a href='www.google.com'>etc</a></li>");
  write("</ol>");
}

//------------------------------------------------------
#define BODY_HTML_FILE_FMT "../pages/%s.html"

void body(char *name) {
  write("<body>");

  size_t filename_sz = strlen(name) + strlen(BODY_HTML_FILE_FMT);
  char *filename = malloc(filename_sz);

  snprintf(filename, filename_sz, BODY_HTML_FILE_FMT, name);

  FILE *body_fp = fopen(filename, "r");

  free(filename);

  if (body_fp != NULL) {
    char ch;
    while ((ch = fgetc(body_fp)) != EOF)
      fputc(ch, fp);

    fclose(body_fp);
  }

  write("</body>");
}

//------------------------------------------------------
#define OUTPUT_HTML_FILE_FMT "../site/%s.html"

void page(char *name) {
  size_t filename_sz = strlen(name) + strlen(OUTPUT_HTML_FILE_FMT);
  char *filename = malloc(filename_sz);

  snprintf(filename, filename_sz, OUTPUT_HTML_FILE_FMT, name);

  fp = fopen(filename, "w");

  printf("filename %s\n", filename);

  free(filename);

  if (fp == NULL) {
    printf("[error] unable to open file for writing!\n");
    return;
  }

  html_open();

  head_open();
  title(replace(name, '_', ' '));
  menu();
  head_close();

  body(name);

  html_close();

  fclose(fp);
}

//------------------------------------------------------
int main(int argc, char **argv) {
  if (argc < 2) {
    printf("[error] must enter a page to compile!\n");
    return -1;
  }

  if (argc > 2)
    theme = argv[2];

  srand(time(NULL));
  page(argv[1]);

  return 0;
}

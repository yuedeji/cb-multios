#ifndef DIFFER_H_
#define DIFFER_H_

void cgc_compare_files(cgc_SFILE *lfile, cgc_SFILE *rfile, int ignore_ws, int treat_as_ascii);
void cgc_clear_cache(int file_num);

#endif

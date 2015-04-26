#ifndef __MMAP_H__

#define FILEPATH "somefile"
#define NUMINTS  (10)
#define FILESIZE (NUMINTS * sizeof(int))

enum
{
   MAP_S = 1, //MAP_SHARED
   MAP_P,     //MAP_PRIVATE
   MAP_A,     //MAP_ANONYMOUS
   MAP_F,     //MAP_FIXED
   MAP_H      //MAP_HUGETLB
};

#endif /* __MMAP_H__ */

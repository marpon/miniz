/*
 ex01_minz.c - Demonstrates minz.cxx's usage
	for z_compress(); z_uncompress() functions
	and mz_compress(); mz_uncompress() functions
*/

#include "minz.cxx"
#include <stdio.h>

typedef unsigned char ubyte;
typedef unsigned int ulong;
typedef unsigned long ulongint;

// The string to compress.
const char *s_pStr = "Good morning Dr. Chandra. This is Hal. I am ready for my first lesson." \
                     "Good morning Dr. Chandra1. This is Hal. I am ready for my first lesson1." \
                     "Good morning Dr. Chandra2. This is Hal. I am ready for my first lesson2." \
                     "Good morning Dr. Chandra3. This is Hal. I am ready for my first lesson3." \
                     "Good morning Dr. Chandra4. This is Hal. I am ready for my first lesson4." \
                     "Good morning Dr. Chandra5. This is Hal. I am ready for my first lesson5." \
                     "Good morning Dr. Chandra6. This is Hal. I am ready for my first lesson6.";

int main()
{
    int cmp_status;
    ulongint src_len = (ulongint)strlen(s_pStr)+1;
    ulongint cmp_len = mz_compressBound(src_len);
    printf("intial input size %u   output buffer size %u bytes\n", (ulong)src_len, (ulong)cmp_len);
    
    ulongint uncomp_len = src_len ;
    ubyte *pCmp, *pUncomp, *pcomp2;
    ubyte *pDest0 ;
    ulongint cmp_len2 = 0;

    cmp_status = z_compress(&pcomp2, &cmp_len2, (const ubyte *)s_pStr, src_len);

    printf("pcomp2 %p    cmp_len2 %u \n", pcomp2, (ulong)cmp_len2);
    printf("uncompressed %u   compressed %u \n",  (ulong)z_UncompBound(pcomp2), (ulong)z_CompBound(pcomp2));

    ulongint src_len1 = 0 ;
    ubyte *pUncomp2;
    cmp_status = z_uncompress(&pUncomp2, &src_len1, (const ubyte *)pcomp2);

    if (cmp_status != MZ_OK)
    {
        printf("uncompress failed!\n");
        free(pUncomp2);
        free(pcomp2);
        return EXIT_FAILURE;
    }

    printf("Decompressed from %u to %u bytes\n", (ulong)z_CompBound(pcomp2), (ulong)src_len1);

    // Ensure uncompress() returned the expected data.
    if ((src_len1 != src_len) || (memcmp(pUncomp2, s_pStr, (size_t)src_len)))
    {
        printf("Decompression failed!\n");
        free(pcomp2);
        free(pUncomp2);
        return EXIT_FAILURE;
    }
    printf("Decompression succeeded!\n");
    
	free(pcomp2);
    free(pUncomp2);


    // Allocate buffers to hold compressed and uncompressed data.
    pCmp = (ubyte *)malloc((size_t)cmp_len);
    pUncomp = (ubyte *)malloc((size_t)uncomp_len);
    pDest0 = pCmp;
    if ((!pCmp) || (!pUncomp))
    {
        printf("Out of memory!\n");
        return EXIT_FAILURE;
    }

    // Compress the string.
    cmp_status = mz_compress(pCmp, &cmp_len, (const ubyte *)s_pStr, src_len);
    printf("pCmp %p    pDest0 %p \n", pCmp, pDest0);

    if (cmp_status != MZ_OK)
    {
        printf("compress() failed!\n");
        free(pCmp);
        free(pUncomp);
        return EXIT_FAILURE;
    }

    printf("Compressed from %u to %u bytes\n", (ulong)src_len, (ulong)cmp_len);

    // Decompress.
    cmp_status = mz_uncompress(pUncomp, &uncomp_len, pCmp, cmp_len);



    if (cmp_status != MZ_OK)
    {
        printf("uncompress failed!\n");
        free(pCmp);
        free(pUncomp);
        return EXIT_FAILURE;
    }

    printf("Decompressed from %u to %u bytes\n", (ulong)cmp_len, (ulong)uncomp_len);

    // Ensure uncompress() returned the expected data.
    if ((uncomp_len != src_len) || (memcmp(pUncomp, s_pStr, (size_t)src_len)))
    {
        printf("Decompression failed!\n");
        free(pCmp);
        free(pUncomp);
        return EXIT_FAILURE;
    }
    printf("Decompression succeeded!\n");

    free(pCmp);
    free(pUncomp);


    printf("Success.\n");
    return EXIT_SUCCESS;
}

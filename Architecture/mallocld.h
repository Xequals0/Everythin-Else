#ifndef mallocld_h
#define mallocld_h

// Detects and reports memory leaks.

#define	malloc( x )	mallocld( x, __FILE__, __LINE__ )
#define	free( x )	freeld( x, __FILE__, __LINE__ )
#define	realloc( x, i )	reallocld( x, i, __FILE__, __LINE__ )

void *		mallocld( unsigned int, const char *, unsigned int );
void		freeld( void *, const char *, unsigned int );
void *		reallocld( void *, unsigned int, const char *, unsigned int );
void		checkheap( const char *, unsigned int );

#endif

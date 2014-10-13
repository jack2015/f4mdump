#ifndef SULGE_F4M_DEBUG
#define SULGE_F4M_DEBUG

//#define DEBUG

#ifdef DEBUG
#define printDBG(...) do{ fprintf( stdout, __VA_ARGS__ ); } while( false )
#else
#define printDBG(...) do{ } while ( false )
#endif

#define printExc(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( false )

#endif /* SULGE_F4M_DEBUG */
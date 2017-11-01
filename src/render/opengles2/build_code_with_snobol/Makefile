
	CFLAGS = -g  -DHAVE_OPENGL=1 


examples: testgles2.exe triangle.exe 

system: SDL_gles2_stubcall_overs_wrapper.c SDL_gles2_stubcall_overs_wrapper.h SDL_gles2funcs.h
	cp  SDL_gles2_stubcall_overs_wrapper.c SDL_gles2_stubcall_overs_wrapper.h SDL_gles2funcs.h ..
	touch done


testgles2.exe: testgles2.c  SDL_gles2_stubcall_overs_wrapper.c SDL_gles2_stubcall_overs_wrapper.h SDL_gles2funcs.h 
	cp $(LOCAL_PREFIX)/lib/libSDL2*.a .
	rm libSDL2.dll.a
	$(CC)  -DHAVE_OPENGL=1   -I$(LOCAL_PREFI) -L.   testgles2.c SDL_gles2_stubcall_overs_wrapper.c \
	  -lmingw32 -lSDL2main -lSDL2_test -lSDL2  -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm \
	  -limm32 -lole32 -loleaut32 -lshell32 \
	    -lversion -luuid -static-libgcc -o testgles2.exe

triangle.exe: triangle.c SDL_gles2_define_overs_wrapper.h SDL_gles2_stubcall_overs_wrapper.c
	cp /usr/local/x86_64-w64-mingw32/lib/libSDL2*.a .
	rm libSDL2.dll.a
	$(CC)  -DHAVE_OPENGL=1   -I(LOCAL_PREFIX) -L.   triangle.c SDL_gles2_stubcall_overs_wrapper.c \
	  -lmingw32 -lSDL2main -lSDL2_test -lSDL2  -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 \
	  -lwinmm -limm32 -lole32 -loleaut32 -lshell32 \
	    -lversion -luuid -static-libgcc -o triangle.exe
	    
SDL_gles2funcs.h: gl2.h
	snobol4 convert_gl2_h_into_gles2funcs_h.sno <gl2.h >SDL_gles2funcs.h

SDL_gles2_define_overs_wrapper.h: SDL_gles2funcs.h
	snobol4 convert_gles2funcs_into_gles2_define_overs_h.sno <SDL_gles2funcs.h >SDL_gles2_define_overs_wrapper.h

SDL_gles2_stubcall_overs_wrapper.h: gl2.h
	snobol4 convert_gl2_h_into_gles2_stubcall_overs_h.sno <gl2.h >SDL_gles2_stubcall_overs_wrapper.h

SDL_gles2_stubcall_overs_wrapper.c: gl2.h
	snobol4 convert_gl2_h_into_gles2_stubcall_overs_c.sno <gl2.h >SDL_gles2_stubcall_overs_wrapper.c
	 

	

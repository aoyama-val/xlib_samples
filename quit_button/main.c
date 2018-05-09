#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int main( int argc, char **argv )
{
     Display *d;
     XTextProperty win_name;    
     Window top, lab, quit;
     GC gc;
     XEvent event;
     char *lab_str = "Xlib(not use ToolKit!)";
     char *quit_str = "Quit";

     /* サーバとの接続 */
     if( (d = XOpenDisplay( NULL )) == NULL ) {
          fprintf( stderr, "Ｘサーバに接続できません\n" );
          return 1;
     }

     /* メインウィンドウの作成 */
     top = XCreateSimpleWindow( d, RootWindow( d, 0 ), 100, 100, 
                                150, 60, 1, 
                              /* 幅 高さ 境界線 .... カスタマイズ不可 */
                                BlackPixel( d, 0 ),
                                WhitePixel( d, 0 ) );

     /* ウィンドウ名などのプロパティのセット(こんなこともしないと....) */
     win_name.value = "XLib";
     win_name.encoding = XA_STRING;
     win_name.format = 8;
     win_name.nitems = strlen( win_name.value );
     XSetWMName( d, top, &win_name );

     /* GC の作成 */
     gc = XCreateGC( d, top, 0, 0 );

     /* サブウィンドウの作成 */
     lab = XCreateSimpleWindow( d, top, 10, 5,
                                140, 20, 0, BlackPixel(d,0), WhitePixel(d,0) );

     quit = XCreateSimpleWindow( d, top, 50, 30,
                                40, 20, 1, BlackPixel(d,0), WhitePixel(d,0) );

     /* 受け取るイベントをサーバに通知 */
     XSelectInput( d, top, ExposureMask );
     XSelectInput( d, quit, ButtonPressMask | EnterWindowMask | LeaveWindowMask );

     /* ウィンドウのマップ */
     XMapSubwindows( d, top );
     XMapWindow( d, top );
     XFlush( d );

     /* イベントループ。XtAppMainLoop() もこんなことをしている*/
     while( 1 ) {
          XNextEvent( d, &event );  /* イベントキューから取り出す */
          switch( event.type ) {
          case Expose:        /* 再描画など */
               if( event.xexpose.count == 0 ) {
                    XDrawString( d, lab, gc, 3, 15, lab_str, strlen(lab_str) );
                    XDrawString( d, quit, gc, 8, 15, quit_str, strlen(quit_str) );
                    XFlush( d );
               }
               break;
          case ButtonPressMask:  /* クリックがなされた */
               return 0;
               break;
          case EnterNotify:     /* マウスがウィンドウに入った */
               XSetWindowBorderWidth( d, quit, 2 );
               XFlush( d );
               break;
          case LeaveNotify:     /* マウスがウィンドウから出た */
               XSetWindowBorderWidth( d, quit, 1 );
               XFlush( d );
               break;
          }
     }
}

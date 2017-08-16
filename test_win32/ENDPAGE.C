
#include <windows.h>  
#include "EndPage.h" 


HINSTANCE hInst;   // current instance

LPCTSTR lpszAppName  = "MyApp";
LPCTSTR lpszTitle    = "My Application"; 


int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPTSTR lpCmdLine, int nCmdShow)
{
   MSG      msg;
   HWND     hWnd; 
   WNDCLASSEX wc;

   // Register the main application window class.
   //............................................
   wc.style         = CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc   = (WNDPROC)WndProc;       
   wc.cbClsExtra    = 0;                      
   wc.cbWndExtra    = 0;                      
   wc.hInstance     = hInstance;              
   wc.hIcon         = LoadIcon( hInstance, lpszAppName ); 
   wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
   wc.lpszMenuName  = lpszAppName;              
   wc.lpszClassName = lpszAppName;              
   wc.cbSize        = sizeof( WNDCLASSEX );
   wc.hIconSm       = LoadImage( hInstance, lpszAppName,
                                 IMAGE_ICON, 16, 16,
                                 LR_DEFAULTCOLOR );

   if ( !RegisterClassEx( &wc ) )
      return( FALSE );

   hInst = hInstance; 

   // Create the main application window.
   //....................................
   hWnd = CreateWindow( lpszAppName, 
                        lpszTitle,    
                        WS_OVERLAPPEDWINDOW, 
                        CW_USEDEFAULT, 0, 
                        CW_USEDEFAULT, 0,  
                        NULL,              
                        NULL,              
                        hInstance,         
                        NULL               
                      );

   if ( !hWnd ) 
      return( FALSE );

   ShowWindow( hWnd, nCmdShow ); 
   UpdateWindow( hWnd );         

   while( GetMessage( &msg, NULL, 0, 0) )   
   {
      TranslateMessage( &msg ); 
      DispatchMessage( &msg );  
   }

   return( msg.wParam ); 
}


LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch( uMsg )
   {
      case WM_COMMAND :
              switch( LOWORD( wParam ) )
              {
                 case IDM_TEST :
                        {
                           HDC     hDC;
                           DOCINFO di;

                           // Create a device context for the
                           // printer to which text will be output.
                           //......................................                       
                           hDC = CreateDC( "WINSPOOL", "Brother MFC-J4510DW Printer", 
                                           NULL, NULL );

                           // Setup the DOCINFO structure.
                           //.............................
                           memset( &di, 0, sizeof( DOCINFO ) );
                           di.cbSize      = sizeof( DOCINFO );
                           di.lpszDocName = "Test Document";

                           // Start the printing.
                           //....................
                           if ( StartDoc( hDC, &di ) != SP_ERROR )
                           {
                               // Print page 1.
                               //..............
                               StartPage( hDC );
                               TextOut( hDC, 10, 10, "This is on Page 1.",18);
                               EndPage( hDC );

                               // Print page 2.
                               //..............
                               StartPage( hDC );
                               TextOut( hDC, 10, 10, "This is on Page 2.",18);
                               EndPage( hDC );

                               // End printing.
                               //..............
                               EndDoc( hDC );
                           }

                           DeleteDC( hDC );
                        }
                        break;

                 case IDM_ABOUT :
                        DialogBox( hInst, "AboutBox", hWnd, (DLGPROC)About );
                        break;

                 case IDM_EXIT :
                        DestroyWindow( hWnd );
                        break;
              }
              break;
      
      case WM_DESTROY :
              PostQuitMessage(0);
              break;

      default :
            return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
   }

   return( 0L );
}


LRESULT CALLBACK About( HWND hDlg,           
                        UINT message,        
                        WPARAM wParam,       
                        LPARAM lParam)
{
   switch (message) 
   {
       case WM_INITDIALOG: 
               return (TRUE);

       case WM_COMMAND:                              
               if (   LOWORD(wParam) == IDOK         
                   || LOWORD(wParam) == IDCANCEL)    
               {
                       EndDialog(hDlg, TRUE);        
                       return (TRUE);
               }
               break;
   }

   return (FALSE); 
}

#include "PchApp.h"

#include "DisplayTab.h"

#include "config.h"

#include "Window.h"
#include "main.h"
#include "ScrollBox.h"
#include "ToolTab.h"
#include "ToolWindow.h"

ScrollBox hDScrollBox;

int DisplayTab::GetContainerHeight()
{
    //return 90 + (20 * toolWindow.bands);
    return 0;
}

int DisplayTab::Create(HWND parent,RECT *parentRect)
{
    ToolTab::Create(parent,parentRect);
    prevProc=SetWindowProcedure(&WindowProcedure);
    
    RECT rect;
   	rect.top=5;
   	rect.left=25;
   	rect.right=218;
   	rect.bottom=217;                 	
	hDScrollBox.Create(GetHandle(),&rect);

    hRed = CreateWindowEx(0, "BUTTON", "R", WS_CHILD | BS_GROUPBOX | WS_VISIBLE , 118, 5,
		26, 20 + (20 * toolWindow.bands), hDScrollBox.GetHandle(), NULL, Window::GetAppInstance(), NULL);
	SetFont(hRed,FONT_BOLD);
	
	hGreen = CreateWindowEx(0, "BUTTON", "G", WS_CHILD | BS_GROUPBOX | WS_VISIBLE, 144, 5,
		26, 20 + (20 * toolWindow.bands), hDScrollBox.GetHandle(), NULL, Window::GetAppInstance(), NULL);
	SetFont(hGreen,FONT_BOLD);	
		
    hBlue = CreateWindowEx(0, "BUTTON", "B", WS_CHILD | BS_GROUPBOX | WS_VISIBLE, 170, 5,
		26, 20 + (20 * toolWindow.bands), hDScrollBox.GetHandle(), NULL, Window::GetAppInstance(), NULL);
	SetFont(hBlue,FONT_BOLD);		

	/* Dynamically add Radio buttons  */
	redRadiobuttons=new HWND[toolWindow.bands];
	greenRadiobuttons=new HWND[toolWindow.bands];
	blueRadiobuttons=new HWND[toolWindow.bands];	

    for (int i=0; i<toolWindow.bands; i++)  
    {
		char tooltip[100];
		redRadiobuttons[i] = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 7, 15 + (20 * i), 16, 16,
			hRed, NULL, hThisInstance, NULL);
		SetBackgroundBrush(redRadiobuttons[i],toolWindow.hTabBrush);
		if (i==0)
			sprintf(tooltip,"Click if you do not wish to display anything in the red channel.\n"); 		
		else
			sprintf(tooltip,"Click to display Band %d in the red channel.\n",i); 
		//CreateTooltip(redRadiobuttons[i],tooltip);	
			
		greenRadiobuttons[i] = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", NULL,
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 7, 15 + (20 * i), 16, 16,
			hGreen, NULL, hThisInstance, NULL);
		if (i==0)
			sprintf(tooltip,"Click if you do not wish to display anything in the green channel.\n"); 		
		else
			sprintf(tooltip,"Click to display Band %d in the green channel.\n",i); 
		//CreateTooltip(greenRadiobuttons[i],tooltip);	
			
		blueRadiobuttons[i] = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", NULL,
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 7, 15 + (20 * i), 16, 16, hBlue, NULL,
			hThisInstance, NULL);
		if (i==0)
			sprintf(tooltip,"Click if you do not wish to display anything in the blue channel.\n"); 		
		else
			sprintf(tooltip,"Click to display Band %d in the blue channel.\n",i); 
		//CreateTooltip(blueRadiobuttons[i],tooltip);	
		
		const char* name;
		if (i>0) { 
    		// add band names to radio buttons
    		name = "";
    		//name = image_handler->get_band_info(i)->getColourInterpretationName();
    		
    		// If Colour name unknown change band name
    		//const char *altName ="No colour name";
    		//if (strcmp(name, "Unknown")==0)
             // name = altName;
                
            // Add band number to band name
            //name = catcstrings( (char*) " - ", (char*) name);
            name = catcstrings( (char*) inttocstring(i), (char*) name);
            name = catcstrings( (char*) "Band ", (char*) name);
        } else
               name = "NONE";

        // Display band name in tool window 
		HWND hstatic=CreateWindowEx(0, szStaticControl, name,
			WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE  | SS_OWNERDRAW, 10, 20 + (20 * i), 100, 18,
			hDScrollBox.GetHandle(), NULL, Window::GetAppInstance(), NULL);
		SetStaticFont(hstatic,STATIC_FONT_NORMAL);
           
		// Insert 'Update' button under radio buttons. Location based on band number 
		hupdate =  CreateWindowEx(0, "BUTTON", "Update", WS_CHILD | WS_VISIBLE, 80,
			247, 80, 25, GetHandle(), NULL, Window::GetAppInstance(), NULL);  
		SetFont(hupdate,FONT_BOLD);
		CreateTooltip(hupdate,"Update Image"); 

    
	}    
    
    // Set sensible defaults based on available bands.
    // 'bands' includes the NONE option at index 0, so max usable index is (bands - 1).
    int maxIndex = toolWindow.bands - 1;
    int rIndex = (maxIndex >= 1) ? 1 : 0;      // prefer band 1, else NONE
    int gIndex = (maxIndex >= 2) ? 2 : rIndex; // prefer band 2, else fall back to R
    int bIndex = (maxIndex >= 3) ? 3 : gIndex; // prefer band 3, else fall back to G

    SendMessage(redRadiobuttons[rIndex],   BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(greenRadiobuttons[gIndex], BM_SETCHECK, BST_CHECKED, 0);
    SendMessage(blueRadiobuttons[bIndex],  BM_SETCHECK, BST_CHECKED, 0);

    hDScrollBox.UpdateScrollBar();
    return true;
}

LRESULT CALLBACK DisplayTab::WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    DisplayTab* win=(DisplayTab*)Window::GetWindowObject(hwnd);
    
    switch(message)
    {
            case WM_COMMAND:
            //if(hupdate==(HWND)lParam)
            {
                // find out which bands are selected
				int r, g, b;
				for (int i=0; i<toolWindow.bands; i++)
   				{
					LRESULT state = SendMessageA(win->redRadiobuttons[i], BM_GETCHECK, 0, 0);
					if(state==BST_CHECKED)
						r = i;
					
					state = SendMessageA(win->greenRadiobuttons[i], BM_GETCHECK, 0, 0);
					if(state==BST_CHECKED)
						g = i;
					
					state = SendMessageA(win->blueRadiobuttons[i], BM_GETCHECK, 0, 0);
					if(state==BST_CHECKED)
						b = i;
				}

                // set image bands to display bands
                assert(image_handler != NULL);
                image_handler->get_image_viewport()->set_display_bands(r,g,b);
            }                
            return 0;
    }
    return CallWindowProc(win->prevProc,hwnd,message,wParam,lParam);    
}

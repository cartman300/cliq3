/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

GAME OPTIONS MENU

=======================================================================
*/


#include "ui_local.h"


#define ART_FRAMEL				"menu/art/frame2_l"
#define ART_FRAMER				"menu/art/frame1_r"
#define ART_BACK0				"menu/art/back_0"
#define ART_BACK1				"menu/art/back_1"

#define PREFERENCES_X_POS		360

#define ID_CROSSHAIR			127
#define ID_SIMPLEITEMS			128
#define ID_HIGHQUALITYSKY		129
#define ID_EJECTINGBRASS		130
#define ID_WALLMARKS			131
#define ID_DYNAMICLIGHTS		132
#define ID_IDENTIFYTARGET		133
#define ID_SYNCEVERYFRAME		134
#define ID_FORCEMODEL			135
#define ID_DRAWTEAMOVERLAY		136
#define ID_ALLOWDOWNLOAD			137
#define ID_BACK					138

#define	NUM_CROSSHAIRS			10


typedef struct {
	menuframework_s		menu;

	menutext_s			banner;
	menubitmap_s		framel;
	menubitmap_s		framer;

	menulist_s			crosshair;
	menuradiobutton_s	simpleitems;
	menuradiobutton_s	brass;
	menuradiobutton_s	wallmarks;
	menuradiobutton_s	dynamiclights;
	menuradiobutton_s	identifytarget;
	menuradiobutton_s	highqualitysky;
	menuradiobutton_s	synceveryframe;
	menuradiobutton_s	forcemodel;
	menulist_s			drawteamoverlay;
	menuradiobutton_s	allowdownload;
	menubitmap_s		back;

	qhandle_t			crosshairShader[NUM_CROSSHAIRS];
} preferences_t;

static preferences_t s_preferences;

static const char *teamoverlay_names[] =
{
	"off",
	"upper right",
	"lower right",
	"lower left",
	NULL
};

static void Preferences_SetMenuItems( void ) {
	s_preferences.crosshair.curvalue		= (int)trap_Cvar_VariableValue( "cg_drawCrosshair" ) % NUM_CROSSHAIRS;
	s_preferences.simpleitems.curvalue		= trap_Cvar_VariableValue( "cg_simpleItems" ) != 0;
	s_preferences.brass.curvalue			= trap_Cvar_VariableValue( "cg_brassTime" ) != 0;
	s_preferences.wallmarks.curvalue		= trap_Cvar_VariableValue( "cg_marks" ) != 0;
	s_preferences.identifytarget.curvalue	= trap_Cvar_VariableValue( "cg_drawCrosshairNames" ) != 0;
	s_preferences.dynamiclights.curvalue	= trap_Cvar_VariableValue( "r_dynamiclight" ) != 0;
	s_preferences.highqualitysky.curvalue	= trap_Cvar_VariableValue ( "r_fastsky" ) == 0;
	s_preferences.synceveryframe.curvalue	= trap_Cvar_VariableValue( "r_finish" ) != 0;
	s_preferences.forcemodel.curvalue		= trap_Cvar_VariableValue( "cg_forcemodel" ) != 0;
	s_preferences.drawteamoverlay.curvalue	= Com_Clamp( 0, 3, trap_Cvar_VariableValue( "cg_drawTeamOverlay" ) );
	s_preferences.allowdownload.curvalue	= trap_Cvar_VariableValue( "cl_allowDownload" ) != 0;
}


static void Preferences_Event( void* ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_CROSSHAIR:
		trap_Cvar_SetValue( "cg_drawCrosshair", s_preferences.crosshair.curvalue );
		break;

	case ID_SIMPLEITEMS:
		trap_Cvar_SetValue( "cg_simpleItems", s_preferences.simpleitems.curvalue );
		break;

	case ID_HIGHQUALITYSKY:
		trap_Cvar_SetValue( "r_fastsky", !s_preferences.highqualitysky.curvalue );
		break;

	case ID_EJECTINGBRASS:
		if ( s_preferences.brass.curvalue )
			trap_Cvar_Reset( "cg_brassTime" );
		else
			trap_Cvar_SetValue( "cg_brassTime", 0 );
		break;

	case ID_WALLMARKS:
		trap_Cvar_SetValue( "cg_marks", s_preferences.wallmarks.curvalue );
		break;

	case ID_DYNAMICLIGHTS:
		trap_Cvar_SetValue( "r_dynamiclight", s_preferences.dynamiclights.curvalue );
		break;		

	case ID_IDENTIFYTARGET:
		trap_Cvar_SetValue( "cg_drawCrosshairNames", s_preferences.identifytarget.curvalue );
		break;

	case ID_SYNCEVERYFRAME:
		trap_Cvar_SetValue( "r_finish", s_preferences.synceveryframe.curvalue );
		break;

	case ID_FORCEMODEL:
		trap_Cvar_SetValue( "cg_forcemodel", s_preferences.forcemodel.curvalue );
		break;

	case ID_DRAWTEAMOVERLAY:
		trap_Cvar_SetValue( "cg_drawTeamOverlay", s_preferences.drawteamoverlay.curvalue );
		break;

	case ID_ALLOWDOWNLOAD:
		trap_Cvar_SetValue( "cl_allowDownload", s_preferences.allowdownload.curvalue );
		trap_Cvar_SetValue( "sv_allowDownload", s_preferences.allowdownload.curvalue );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
=================
Crosshair_Draw
=================
*/
static void Crosshair_Draw( void *self ) {
	menulist_s	*s;
	float		*color;
	int			x, y;
	int			style;
	qboolean	focus;

	s = (menulist_s *)self;
	x = s->Generic.x;
	y =	s->Generic.y;

	style = UI_SMALLFONT;
	focus = (s->Generic.parent->cursor == s->Generic.menuPosition);

	if ( s->Generic.flags & QMF_GRAYED )
		color = text_color_disabled;
	else if ( focus )
	{
		color = text_color_highlight;
		style |= UI_PULSE;
	}
	else if ( s->Generic.flags & QMF_BLINK )
	{
		color = text_color_highlight;
		style |= UI_BLINK;
	}
	else
		color = text_color_normal;

	if ( focus )
	{
		// draw cursor
		UI_FillRect( s->Generic.left, s->Generic.top, s->Generic.right-s->Generic.left+1, s->Generic.bottom-s->Generic.top+1, listbar_color ); 
		UI_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color);
	}

	UI_DrawString( x - SMALLCHAR_WIDTH, y, s->Generic.name, style|UI_RIGHT, color );
	if( !s->curvalue ) {
		return;
	}
	UI_DrawHandlePic( x + SMALLCHAR_WIDTH, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue] );
}


static void Preferences_MenuInit( void ) {
	int				y;

	memset( &s_preferences, 0 ,sizeof(preferences_t) );

	Preferences_Cache();

	s_preferences.menu.wrapAround = qtrue;
	s_preferences.menu.fullscreen = qtrue;

	s_preferences.banner.Generic.type  = MTYPE_BTEXT;
	s_preferences.banner.Generic.x	   = 320;
	s_preferences.banner.Generic.y	   = 16;
	s_preferences.banner.string		   = "GAME OPTIONS";
	s_preferences.banner.color         = color_white;
	s_preferences.banner.style         = UI_CENTER;

	s_preferences.framel.Generic.type  = MTYPE_BITMAP;
	s_preferences.framel.Generic.name  = ART_FRAMEL;
	s_preferences.framel.Generic.flags = QMF_INACTIVE;
	s_preferences.framel.Generic.x	   = 0;
	s_preferences.framel.Generic.y	   = 78;
	s_preferences.framel.width  	   = 256;
	s_preferences.framel.height  	   = 329;

	s_preferences.framer.Generic.type  = MTYPE_BITMAP;
	s_preferences.framer.Generic.name  = ART_FRAMER;
	s_preferences.framer.Generic.flags = QMF_INACTIVE;
	s_preferences.framer.Generic.x	   = 376;
	s_preferences.framer.Generic.y	   = 76;
	s_preferences.framer.width  	   = 256;
	s_preferences.framer.height  	   = 334;

	y = 144;
	s_preferences.crosshair.Generic.type		= MTYPE_SPINCONTROL;
	s_preferences.crosshair.Generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshair.Generic.x			= PREFERENCES_X_POS;
	s_preferences.crosshair.Generic.y			= y;
	s_preferences.crosshair.Generic.name		= "Crosshair:";
	s_preferences.crosshair.Generic.callback	= Preferences_Event;
	s_preferences.crosshair.Generic.ownerdraw	= Crosshair_Draw;
	s_preferences.crosshair.Generic.id			= ID_CROSSHAIR;
	s_preferences.crosshair.Generic.top			= y - 4;
	s_preferences.crosshair.Generic.bottom		= y + 20;
	s_preferences.crosshair.Generic.left		= PREFERENCES_X_POS - ( ( strlen(s_preferences.crosshair.Generic.name) + 1 ) * SMALLCHAR_WIDTH );
	s_preferences.crosshair.Generic.right		= PREFERENCES_X_POS + 48;
	s_preferences.crosshair.numitems			= NUM_CROSSHAIRS;

	y += BIGCHAR_HEIGHT+2+4;
	s_preferences.simpleitems.Generic.type        = MTYPE_RADIOBUTTON;
	s_preferences.simpleitems.Generic.name	      = "Simple Items:";
	s_preferences.simpleitems.Generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.simpleitems.Generic.callback    = Preferences_Event;
	s_preferences.simpleitems.Generic.id          = ID_SIMPLEITEMS;
	s_preferences.simpleitems.Generic.x	          = PREFERENCES_X_POS;
	s_preferences.simpleitems.Generic.y	          = y;

	y += BIGCHAR_HEIGHT;
	s_preferences.wallmarks.Generic.type          = MTYPE_RADIOBUTTON;
	s_preferences.wallmarks.Generic.name	      = "Marks on Walls:";
	s_preferences.wallmarks.Generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.wallmarks.Generic.callback      = Preferences_Event;
	s_preferences.wallmarks.Generic.id            = ID_WALLMARKS;
	s_preferences.wallmarks.Generic.x	          = PREFERENCES_X_POS;
	s_preferences.wallmarks.Generic.y	          = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.brass.Generic.type              = MTYPE_RADIOBUTTON;
	s_preferences.brass.Generic.name	          = "Ejecting Brass:";
	s_preferences.brass.Generic.flags	          = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.brass.Generic.callback          = Preferences_Event;
	s_preferences.brass.Generic.id                = ID_EJECTINGBRASS;
	s_preferences.brass.Generic.x	              = PREFERENCES_X_POS;
	s_preferences.brass.Generic.y	              = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.dynamiclights.Generic.type      = MTYPE_RADIOBUTTON;
	s_preferences.dynamiclights.Generic.name	  = "Dynamic Lights:";
	s_preferences.dynamiclights.Generic.flags     = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.dynamiclights.Generic.callback  = Preferences_Event;
	s_preferences.dynamiclights.Generic.id        = ID_DYNAMICLIGHTS;
	s_preferences.dynamiclights.Generic.x	      = PREFERENCES_X_POS;
	s_preferences.dynamiclights.Generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.identifytarget.Generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.identifytarget.Generic.name	  = "Identify Target:";
	s_preferences.identifytarget.Generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.identifytarget.Generic.callback = Preferences_Event;
	s_preferences.identifytarget.Generic.id       = ID_IDENTIFYTARGET;
	s_preferences.identifytarget.Generic.x	      = PREFERENCES_X_POS;
	s_preferences.identifytarget.Generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.highqualitysky.Generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.highqualitysky.Generic.name	  = "High Quality Sky:";
	s_preferences.highqualitysky.Generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.highqualitysky.Generic.callback = Preferences_Event;
	s_preferences.highqualitysky.Generic.id       = ID_HIGHQUALITYSKY;
	s_preferences.highqualitysky.Generic.x	      = PREFERENCES_X_POS;
	s_preferences.highqualitysky.Generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.synceveryframe.Generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.synceveryframe.Generic.name	  = "Sync Every Frame:";
	s_preferences.synceveryframe.Generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.synceveryframe.Generic.callback = Preferences_Event;
	s_preferences.synceveryframe.Generic.id       = ID_SYNCEVERYFRAME;
	s_preferences.synceveryframe.Generic.x	      = PREFERENCES_X_POS;
	s_preferences.synceveryframe.Generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.forcemodel.Generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.forcemodel.Generic.name	  = "Force Player Models:";
	s_preferences.forcemodel.Generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.forcemodel.Generic.callback = Preferences_Event;
	s_preferences.forcemodel.Generic.id       = ID_FORCEMODEL;
	s_preferences.forcemodel.Generic.x	      = PREFERENCES_X_POS;
	s_preferences.forcemodel.Generic.y	      = y;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.drawteamoverlay.Generic.type     = MTYPE_SPINCONTROL;
	s_preferences.drawteamoverlay.Generic.name	   = "Draw Team Overlay:";
	s_preferences.drawteamoverlay.Generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.drawteamoverlay.Generic.callback = Preferences_Event;
	s_preferences.drawteamoverlay.Generic.id       = ID_DRAWTEAMOVERLAY;
	s_preferences.drawteamoverlay.Generic.x	       = PREFERENCES_X_POS;
	s_preferences.drawteamoverlay.Generic.y	       = y;
	s_preferences.drawteamoverlay.itemnames			= teamoverlay_names;

	y += BIGCHAR_HEIGHT+2;
	s_preferences.allowdownload.Generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.allowdownload.Generic.name	   = "Automatic Downloading:";
	s_preferences.allowdownload.Generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.allowdownload.Generic.callback = Preferences_Event;
	s_preferences.allowdownload.Generic.id       = ID_ALLOWDOWNLOAD;
	s_preferences.allowdownload.Generic.x	       = PREFERENCES_X_POS;
	s_preferences.allowdownload.Generic.y	       = y;

	s_preferences.back.Generic.type	    = MTYPE_BITMAP;
	s_preferences.back.Generic.name     = ART_BACK0;
	s_preferences.back.Generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.back.Generic.callback = Preferences_Event;
	s_preferences.back.Generic.id	    = ID_BACK;
	s_preferences.back.Generic.x		= 0;
	s_preferences.back.Generic.y		= 480-64;
	s_preferences.back.width  		    = 128;
	s_preferences.back.height  		    = 64;
	s_preferences.back.focuspic         = ART_BACK1;

	Menu_AddItem( &s_preferences.menu, &s_preferences.banner );
	Menu_AddItem( &s_preferences.menu, &s_preferences.framel );
	Menu_AddItem( &s_preferences.menu, &s_preferences.framer );

	Menu_AddItem( &s_preferences.menu, &s_preferences.crosshair );
	Menu_AddItem( &s_preferences.menu, &s_preferences.simpleitems );
	Menu_AddItem( &s_preferences.menu, &s_preferences.wallmarks );
	Menu_AddItem( &s_preferences.menu, &s_preferences.brass );
	Menu_AddItem( &s_preferences.menu, &s_preferences.dynamiclights );
	Menu_AddItem( &s_preferences.menu, &s_preferences.identifytarget );
	Menu_AddItem( &s_preferences.menu, &s_preferences.highqualitysky );
	Menu_AddItem( &s_preferences.menu, &s_preferences.synceveryframe );
	Menu_AddItem( &s_preferences.menu, &s_preferences.forcemodel );
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawteamoverlay );
	Menu_AddItem( &s_preferences.menu, &s_preferences.allowdownload );

	Menu_AddItem( &s_preferences.menu, &s_preferences.back );

	Preferences_SetMenuItems();
}


/*
===============
Preferences_Cache
===============
*/
void Preferences_Cache( void ) {
	int		n;

	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	for( n = 0; n < NUM_CROSSHAIRS; n++ ) {
		s_preferences.crosshairShader[n] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a' + n ) );
	}
}


/*
===============
UI_PreferencesMenu
===============
*/
void UI_PreferencesMenu( void ) {
	Preferences_MenuInit();
	UI_PushMenu( &s_preferences.menu );
}

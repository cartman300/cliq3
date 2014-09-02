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

SOUND OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"


#define ART_FRAMEL			"menu/art/frame2_l"
#define ART_FRAMER			"menu/art/frame1_r"
#define ART_BACK0			"menu/art/back_0"
#define ART_BACK1			"menu/art/back_1"
#define ART_ACCEPT0			"menu/art/accept_0"
#define ART_ACCEPT1			"menu/art/accept_1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_EFFECTSVOLUME	14
#define ID_MUSICVOLUME		15
#define ID_QUALITY			16
#define ID_SOUNDSYSTEM		17
//#define ID_A3D				18
#define ID_BACK				19
#define ID_APPLY			20

#define DEFAULT_SDL_SND_SPEED 22050

static const char *quality_items[] = {
	"Low", "Medium", "High", NULL
};

#define UISND_SDL 0
#define UISND_OPENAL 1

static const char *soundSystem_items[] = {
	"SDL", "OpenAL", NULL
};

typedef struct {
	menuframework_s		menu;

	menutext_s			banner;
	menubitmap_s		framel;
	menubitmap_s		framer;

	menutext_s			graphics;
	menutext_s			display;
	menutext_s			sound;
	menutext_s			network;

	menuslider_s		sfxvolume;
	menuslider_s		musicvolume;
	menulist_s  		soundSystem;
	menulist_s			quality;
//	menuradiobutton_s	a3d;

	menubitmap_s		back;
	menubitmap_s		apply;

	float				sfxvolume_original;
	float				musicvolume_original;
	int					soundSystem_original;
	int					quality_original;
} soundOptionsInfo_t;

static soundOptionsInfo_t	soundOptionsInfo;


/*
=================
UI_SoundOptionsMenu_Event
=================
*/
static void UI_SoundOptionsMenu_Event( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;
/*
	case ID_A3D:
		if( soundOptionsInfo.a3d.curvalue ) {
			trap_Cmd_ExecuteText( EXEC_NOW, "s_enable_a3d\n" );
		}
		else {
			trap_Cmd_ExecuteText( EXEC_NOW, "s_disable_a3d\n" );
		}
		soundOptionsInfo.a3d.curvalue = (int)trap_Cvar_VariableValue( "s_usingA3D" );
		break;
*/
	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_APPLY:
		trap_Cvar_SetValue( "s_volume", soundOptionsInfo.sfxvolume.curvalue / 10 );
		soundOptionsInfo.sfxvolume_original = soundOptionsInfo.sfxvolume.curvalue;

		trap_Cvar_SetValue( "s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10 );
		soundOptionsInfo.musicvolume_original = soundOptionsInfo.musicvolume.curvalue;

		// Check if something changed that requires the sound system to be restarted.
		if (soundOptionsInfo.quality_original != soundOptionsInfo.quality.curvalue
			|| soundOptionsInfo.soundSystem_original != soundOptionsInfo.soundSystem.curvalue)
		{
			int speed;

			switch ( soundOptionsInfo.quality.curvalue )
			{
				default:
				case 0:
					speed = 11025;
					break;
				case 1:
					speed = 22050;
					break;
				case 2:
					speed = 44100;
					break;
			}

			if (speed == DEFAULT_SDL_SND_SPEED)
				speed = 0;

			trap_Cvar_SetValue( "s_sdlSpeed", speed );
			soundOptionsInfo.quality_original = soundOptionsInfo.quality.curvalue;

			trap_Cvar_SetValue( "s_useOpenAL", (soundOptionsInfo.soundSystem.curvalue == UISND_OPENAL) );
			soundOptionsInfo.soundSystem_original = soundOptionsInfo.soundSystem.curvalue;

			UI_ForceMenuOff();
			trap_Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );
		}
		break;
	}
}

/*
=================
SoundOptions_UpdateMenuItems
=================
*/
static void SoundOptions_UpdateMenuItems( void )
{
	if ( soundOptionsInfo.soundSystem.curvalue == UISND_SDL )
	{
		soundOptionsInfo.quality.Generic.flags &= ~QMF_GRAYED;
	}
	else
	{
		soundOptionsInfo.quality.Generic.flags |= QMF_GRAYED;
	}

	soundOptionsInfo.apply.Generic.flags |= QMF_HIDDEN|QMF_INACTIVE;

	if ( soundOptionsInfo.sfxvolume_original != soundOptionsInfo.sfxvolume.curvalue )
	{
		soundOptionsInfo.apply.Generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( soundOptionsInfo.musicvolume_original != soundOptionsInfo.musicvolume.curvalue )
	{
		soundOptionsInfo.apply.Generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( soundOptionsInfo.soundSystem_original != soundOptionsInfo.soundSystem.curvalue )
	{
		soundOptionsInfo.apply.Generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( soundOptionsInfo.quality_original != soundOptionsInfo.quality.curvalue )
	{
		soundOptionsInfo.apply.Generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
}

/*
================
SoundOptions_MenuDraw
================
*/
void SoundOptions_MenuDraw (void)
{
//APSFIX - rework this
	SoundOptions_UpdateMenuItems();

	Menu_Draw( &soundOptionsInfo.menu );
}

/*
===============
UI_SoundOptionsMenu_Init
===============
*/
static void UI_SoundOptionsMenu_Init( void ) {
	int				y;
	int				speed;

	memset( &soundOptionsInfo, 0, sizeof(soundOptionsInfo) );

	UI_SoundOptionsMenu_Cache();
	soundOptionsInfo.menu.wrapAround = qtrue;
	soundOptionsInfo.menu.fullscreen = qtrue;
	soundOptionsInfo.menu.draw		= SoundOptions_MenuDraw;

	soundOptionsInfo.banner.Generic.type		= MTYPE_BTEXT;
	soundOptionsInfo.banner.Generic.flags		= QMF_CENTER_JUSTIFY;
	soundOptionsInfo.banner.Generic.x			= 320;
	soundOptionsInfo.banner.Generic.y			= 16;
	soundOptionsInfo.banner.string				= "SYSTEM SETUP";
	soundOptionsInfo.banner.color				= color_white;
	soundOptionsInfo.banner.style				= UI_CENTER;

	soundOptionsInfo.framel.Generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.framel.Generic.name		= ART_FRAMEL;
	soundOptionsInfo.framel.Generic.flags		= QMF_INACTIVE;
	soundOptionsInfo.framel.Generic.x			= 0;  
	soundOptionsInfo.framel.Generic.y			= 78;
	soundOptionsInfo.framel.width				= 256;
	soundOptionsInfo.framel.height				= 329;

	soundOptionsInfo.framer.Generic.type		= MTYPE_BITMAP;
	soundOptionsInfo.framer.Generic.name		= ART_FRAMER;
	soundOptionsInfo.framer.Generic.flags		= QMF_INACTIVE;
	soundOptionsInfo.framer.Generic.x			= 376;
	soundOptionsInfo.framer.Generic.y			= 76;
	soundOptionsInfo.framer.width				= 256;
	soundOptionsInfo.framer.height				= 334;

	soundOptionsInfo.graphics.Generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.graphics.Generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.graphics.Generic.id		= ID_GRAPHICS;
	soundOptionsInfo.graphics.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.graphics.Generic.x			= 216;
	soundOptionsInfo.graphics.Generic.y			= 240 - 2 * PROP_HEIGHT;
	soundOptionsInfo.graphics.string			= "GRAPHICS";
	soundOptionsInfo.graphics.style				= UI_RIGHT;
	soundOptionsInfo.graphics.color				= color_red;

	soundOptionsInfo.display.Generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.display.Generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.display.Generic.id			= ID_DISPLAY;
	soundOptionsInfo.display.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.display.Generic.x			= 216;
	soundOptionsInfo.display.Generic.y			= 240 - PROP_HEIGHT;
	soundOptionsInfo.display.string				= "DISPLAY";
	soundOptionsInfo.display.style				= UI_RIGHT;
	soundOptionsInfo.display.color				= color_red;

	soundOptionsInfo.sound.Generic.type			= MTYPE_PTEXT;
	soundOptionsInfo.sound.Generic.flags		= QMF_RIGHT_JUSTIFY;
	soundOptionsInfo.sound.Generic.id			= ID_SOUND;
	soundOptionsInfo.sound.Generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sound.Generic.x			= 216;
	soundOptionsInfo.sound.Generic.y			= 240;
	soundOptionsInfo.sound.string				= "SOUND";
	soundOptionsInfo.sound.style				= UI_RIGHT;
	soundOptionsInfo.sound.color				= color_red;

	soundOptionsInfo.network.Generic.type		= MTYPE_PTEXT;
	soundOptionsInfo.network.Generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.network.Generic.id			= ID_NETWORK;
	soundOptionsInfo.network.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.network.Generic.x			= 216;
	soundOptionsInfo.network.Generic.y			= 240 + PROP_HEIGHT;
	soundOptionsInfo.network.string				= "NETWORK";
	soundOptionsInfo.network.style				= UI_RIGHT;
	soundOptionsInfo.network.color				= color_red;

	y = 240 - 2 * (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.sfxvolume.Generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.sfxvolume.Generic.name		= "Effects Volume:";
	soundOptionsInfo.sfxvolume.Generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sfxvolume.Generic.id		= ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.Generic.x		= 400;
	soundOptionsInfo.sfxvolume.Generic.y		= y;
	soundOptionsInfo.sfxvolume.minvalue			= 0;
	soundOptionsInfo.sfxvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.musicvolume.Generic.type		= MTYPE_SLIDER;
	soundOptionsInfo.musicvolume.Generic.name		= "Music Volume:";
	soundOptionsInfo.musicvolume.Generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicvolume.Generic.id			= ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.Generic.x			= 400;
	soundOptionsInfo.musicvolume.Generic.y			= y;
	soundOptionsInfo.musicvolume.minvalue			= 0;
	soundOptionsInfo.musicvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.soundSystem.Generic.type		= MTYPE_SPINCONTROL;
	soundOptionsInfo.soundSystem.Generic.name		= "Sound System:";
	soundOptionsInfo.soundSystem.Generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.soundSystem.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.soundSystem.Generic.id			= ID_SOUNDSYSTEM;
	soundOptionsInfo.soundSystem.Generic.x			= 400;
	soundOptionsInfo.soundSystem.Generic.y			= y;
	soundOptionsInfo.soundSystem.itemnames			= soundSystem_items;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.quality.Generic.type		= MTYPE_SPINCONTROL;
	soundOptionsInfo.quality.Generic.name		= "SDL Sound Quality:";
	soundOptionsInfo.quality.Generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.quality.Generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.quality.Generic.id			= ID_QUALITY;
	soundOptionsInfo.quality.Generic.x			= 400;
	soundOptionsInfo.quality.Generic.y			= y;
	soundOptionsInfo.quality.itemnames			= quality_items;

/*
	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.a3d.Generic.type			= MTYPE_RADIOBUTTON;
	soundOptionsInfo.a3d.Generic.name			= "A3D:";
	soundOptionsInfo.a3d.Generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.a3d.Generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.a3d.Generic.id				= ID_A3D;
	soundOptionsInfo.a3d.Generic.x				= 400;
	soundOptionsInfo.a3d.Generic.y				= y;
*/
	soundOptionsInfo.back.Generic.type			= MTYPE_BITMAP;
	soundOptionsInfo.back.Generic.name			= ART_BACK0;
	soundOptionsInfo.back.Generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.Generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.back.Generic.id			= ID_BACK;
	soundOptionsInfo.back.Generic.x				= 0;
	soundOptionsInfo.back.Generic.y				= 480-64;
	soundOptionsInfo.back.width					= 128;
	soundOptionsInfo.back.height				= 64;
	soundOptionsInfo.back.focuspic				= ART_BACK1;

	soundOptionsInfo.apply.Generic.type			= MTYPE_BITMAP;
	soundOptionsInfo.apply.Generic.name			= ART_ACCEPT0;
	soundOptionsInfo.apply.Generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_HIDDEN|QMF_INACTIVE;
	soundOptionsInfo.apply.Generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.apply.Generic.id			= ID_APPLY;
	soundOptionsInfo.apply.Generic.x			= 640;
	soundOptionsInfo.apply.Generic.y			= 480-64;
	soundOptionsInfo.apply.width				= 128;
	soundOptionsInfo.apply.height				= 64;
	soundOptionsInfo.apply.focuspic				= ART_ACCEPT1;

	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.banner );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.framel );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.framer );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.graphics );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.display );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sound );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.network );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sfxvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.musicvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.soundSystem );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.quality );
//	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.a3d );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.back );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.apply );

	soundOptionsInfo.sfxvolume.curvalue = soundOptionsInfo.sfxvolume_original = trap_Cvar_VariableValue( "s_volume" ) * 10;
	soundOptionsInfo.musicvolume.curvalue = soundOptionsInfo.musicvolume_original = trap_Cvar_VariableValue( "s_musicvolume" ) * 10;

	if (trap_Cvar_VariableValue( "s_useOpenAL" ))
		soundOptionsInfo.soundSystem_original = UISND_OPENAL;
	else
		soundOptionsInfo.soundSystem_original = UISND_SDL;

	soundOptionsInfo.soundSystem.curvalue = soundOptionsInfo.soundSystem_original;

	speed = trap_Cvar_VariableValue( "s_sdlSpeed" );
	if (!speed) // Check for default
		speed = DEFAULT_SDL_SND_SPEED;

	if (speed <= 11025)
		soundOptionsInfo.quality_original = 0;
	else if (speed <= 22050)
		soundOptionsInfo.quality_original = 1;
	else // 44100
		soundOptionsInfo.quality_original = 2;
	soundOptionsInfo.quality.curvalue = soundOptionsInfo.quality_original;

//	soundOptionsInfo.a3d.curvalue = (int)trap_Cvar_VariableValue( "s_usingA3D" );
}


/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void UI_SoundOptionsMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_FRAMEL );
	trap_R_RegisterShaderNoMip( ART_FRAMER );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_ACCEPT0 );
	trap_R_RegisterShaderNoMip( ART_ACCEPT1 );
}


/*
===============
UI_SoundOptionsMenu
===============
*/
void UI_SoundOptionsMenu( void ) {
	UI_SoundOptionsMenu_Init();
	UI_PushMenu( &soundOptionsInfo.menu );
	Menu_SetCursorToItem( &soundOptionsInfo.menu, &soundOptionsInfo.sound );
}

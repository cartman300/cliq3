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

REMOVE BOTS MENU

=======================================================================
*/


#include "ui_local.h"


#define ART_BACKGROUND		"menu/art/addbotframe"
#define ART_BACK0			"menu/art/back_0"
#define ART_BACK1			"menu/art/back_1"	
#define ART_DELETE0			"menu/art/delete_0"
#define ART_DELETE1			"menu/art/delete_1"
#define ART_ARROWS			"menu/art/arrows_vert_0"
#define ART_ARROWUP			"menu/art/arrows_vert_top"
#define ART_ARROWDOWN		"menu/art/arrows_vert_bot"

#define ID_UP				10
#define ID_DOWN				11
#define ID_DELETE			12
#define ID_BACK				13
#define ID_BOTNAME0			20
#define ID_BOTNAME1			21
#define ID_BOTNAME2			22
#define ID_BOTNAME3			23
#define ID_BOTNAME4			24
#define ID_BOTNAME5			25
#define ID_BOTNAME6			26


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	background;

	menubitmap_s	arrows;
	menubitmap_s	up;
	menubitmap_s	down;

	menutext_s		bots[7];

	menubitmap_s	Delete;
	menubitmap_s	back;

	int				numBots;
	int				baseBotNum;
	int				selectedBotNum;
	char			botnames[7][32];
	int				botClientNums[MAX_BOTS];
} removeBotsMenuInfo_t;

static removeBotsMenuInfo_t	removeBotsMenuInfo;


/*
=================
UI_RemoveBotsMenu_SetBotNames
=================
*/
static void UI_RemoveBotsMenu_SetBotNames( void ) {
	int		n;
	char	info[MAX_INFO_STRING];

	for ( n = 0; (n < 7) && (removeBotsMenuInfo.baseBotNum + n < removeBotsMenuInfo.numBots); n++ ) {
		trap_GetConfigString( CS_PLAYERS + removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + n], info, MAX_INFO_STRING );
		Q_strncpyz( removeBotsMenuInfo.botnames[n], Info_ValueForKey( info, "n" ), sizeof(removeBotsMenuInfo.botnames[n]) );
		Q_CleanStr( removeBotsMenuInfo.botnames[n] );
	}

}


/*
=================
UI_RemoveBotsMenu_DeleteEvent
=================
*/
static void UI_RemoveBotsMenu_DeleteEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, va("clientkick %i\n", removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.baseBotNum + removeBotsMenuInfo.selectedBotNum]) );
}


/*
=================
UI_RemoveBotsMenu_BotEvent
=================
*/
static void UI_RemoveBotsMenu_BotEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = color_orange;
	removeBotsMenuInfo.selectedBotNum = ((menucommon_s*)ptr)->id - ID_BOTNAME0;
	removeBotsMenuInfo.bots[removeBotsMenuInfo.selectedBotNum].color = color_white;
}


/*
=================
UI_RemoveAddBotsMenu_BackEvent
=================
*/
static void UI_RemoveBotsMenu_BackEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}
	UI_PopMenu();
}


/*
=================
UI_RemoveBotsMenu_UpEvent
=================
*/
static void UI_RemoveBotsMenu_UpEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if( removeBotsMenuInfo.baseBotNum > 0 ) {
		removeBotsMenuInfo.baseBotNum--;
		UI_RemoveBotsMenu_SetBotNames();
	}
}


/*
=================
UI_RemoveBotsMenu_DownEvent
=================
*/
static void UI_RemoveBotsMenu_DownEvent( void* ptr, int event ) {
	if (event != QM_ACTIVATED) {
		return;
	}

	if( removeBotsMenuInfo.baseBotNum + 7 < removeBotsMenuInfo.numBots ) {
		removeBotsMenuInfo.baseBotNum++;
		UI_RemoveBotsMenu_SetBotNames();
	}
}


/*
=================
UI_RemoveBotsMenu_GetBots
=================
*/
static void UI_RemoveBotsMenu_GetBots( void ) {
	int		numPlayers;
	int		isBot;
	int		n;
	char	info[MAX_INFO_STRING];

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	numPlayers = atoi( Info_ValueForKey( info, "sv_maxclients" ) );
	removeBotsMenuInfo.numBots = 0;

	for( n = 0; n < numPlayers; n++ ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );

		isBot = atoi( Info_ValueForKey( info, "skill" ) );
		if( !isBot ) {
			continue;
		}

		removeBotsMenuInfo.botClientNums[removeBotsMenuInfo.numBots] = n;
		removeBotsMenuInfo.numBots++;
	}
}


/*
=================
UI_RemoveBots_Cache
=================
*/
void UI_RemoveBots_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACKGROUND );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_DELETE0 );
	trap_R_RegisterShaderNoMip( ART_DELETE1 );
}


/*
=================
UI_RemoveBotsMenu_Init
=================
*/
static void UI_RemoveBotsMenu_Init( void ) {
	int		n;
	int		count;
	int		y;

	memset( &removeBotsMenuInfo, 0 ,sizeof(removeBotsMenuInfo) );
	removeBotsMenuInfo.menu.fullscreen = qfalse;
	removeBotsMenuInfo.menu.wrapAround = qtrue;

	UI_RemoveBots_Cache();

	UI_RemoveBotsMenu_GetBots();
	UI_RemoveBotsMenu_SetBotNames();
	count = removeBotsMenuInfo.numBots < 7 ? removeBotsMenuInfo.numBots : 7;

	removeBotsMenuInfo.banner.Generic.type		= MTYPE_BTEXT;
	removeBotsMenuInfo.banner.Generic.x			= 320;
	removeBotsMenuInfo.banner.Generic.y			= 16;
	removeBotsMenuInfo.banner.string			= "REMOVE BOTS";
	removeBotsMenuInfo.banner.color				= color_white;
	removeBotsMenuInfo.banner.style				= UI_CENTER;

	removeBotsMenuInfo.background.Generic.type	= MTYPE_BITMAP;
	removeBotsMenuInfo.background.Generic.name	= ART_BACKGROUND;
	removeBotsMenuInfo.background.Generic.flags	= QMF_INACTIVE;
	removeBotsMenuInfo.background.Generic.x		= 320-233;
	removeBotsMenuInfo.background.Generic.y		= 240-166;
	removeBotsMenuInfo.background.width			= 466;
	removeBotsMenuInfo.background.height		= 332;

	removeBotsMenuInfo.arrows.Generic.type		= MTYPE_BITMAP;
	removeBotsMenuInfo.arrows.Generic.name		= ART_ARROWS;
	removeBotsMenuInfo.arrows.Generic.flags		= QMF_INACTIVE;
	removeBotsMenuInfo.arrows.Generic.x			= 200;
	removeBotsMenuInfo.arrows.Generic.y			= 128;
	removeBotsMenuInfo.arrows.width				= 64;
	removeBotsMenuInfo.arrows.height			= 128;

	removeBotsMenuInfo.up.Generic.type			= MTYPE_BITMAP;
	removeBotsMenuInfo.up.Generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.up.Generic.x				= 200;
	removeBotsMenuInfo.up.Generic.y				= 128;
	removeBotsMenuInfo.up.Generic.id			= ID_UP;
	removeBotsMenuInfo.up.Generic.callback		= UI_RemoveBotsMenu_UpEvent;
	removeBotsMenuInfo.up.width					= 64;
	removeBotsMenuInfo.up.height				= 64;
	removeBotsMenuInfo.up.focuspic				= ART_ARROWUP;

	removeBotsMenuInfo.down.Generic.type		= MTYPE_BITMAP;
	removeBotsMenuInfo.down.Generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.down.Generic.x			= 200;
	removeBotsMenuInfo.down.Generic.y			= 128+64;
	removeBotsMenuInfo.down.Generic.id			= ID_DOWN;
	removeBotsMenuInfo.down.Generic.callback	= UI_RemoveBotsMenu_DownEvent;
	removeBotsMenuInfo.down.width				= 64;
	removeBotsMenuInfo.down.height				= 64;
	removeBotsMenuInfo.down.focuspic			= ART_ARROWDOWN;

	for( n = 0, y = 120; n < count; n++, y += 20 ) {
		removeBotsMenuInfo.bots[n].Generic.type		= MTYPE_PTEXT;
		removeBotsMenuInfo.bots[n].Generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
		removeBotsMenuInfo.bots[n].Generic.id		= ID_BOTNAME0 + n;
		removeBotsMenuInfo.bots[n].Generic.x		= 320 - 56;
		removeBotsMenuInfo.bots[n].Generic.y		= y;
		removeBotsMenuInfo.bots[n].Generic.callback	= UI_RemoveBotsMenu_BotEvent;
		removeBotsMenuInfo.bots[n].string			= removeBotsMenuInfo.botnames[n];
		removeBotsMenuInfo.bots[n].color			= color_orange;
		removeBotsMenuInfo.bots[n].style			= UI_LEFT|UI_SMALLFONT;
	}

	removeBotsMenuInfo.Delete.Generic.type		= MTYPE_BITMAP;
	removeBotsMenuInfo.Delete.Generic.name		= ART_DELETE0;
	removeBotsMenuInfo.Delete.Generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.Delete.Generic.id		= ID_DELETE;
	removeBotsMenuInfo.Delete.Generic.callback	= UI_RemoveBotsMenu_DeleteEvent;
	removeBotsMenuInfo.Delete.Generic.x			= 320+128-128;
	removeBotsMenuInfo.Delete.Generic.y			= 256+128-64;
	removeBotsMenuInfo.Delete.width  			= 128;
	removeBotsMenuInfo.Delete.height  			= 64;
	removeBotsMenuInfo.Delete.focuspic			= ART_DELETE1;

	removeBotsMenuInfo.back.Generic.type		= MTYPE_BITMAP;
	removeBotsMenuInfo.back.Generic.name		= ART_BACK0;
	removeBotsMenuInfo.back.Generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	removeBotsMenuInfo.back.Generic.id			= ID_BACK;
	removeBotsMenuInfo.back.Generic.callback	= UI_RemoveBotsMenu_BackEvent;
	removeBotsMenuInfo.back.Generic.x			= 320-128;
	removeBotsMenuInfo.back.Generic.y			= 256+128-64;
	removeBotsMenuInfo.back.width				= 128;
	removeBotsMenuInfo.back.height				= 64;
	removeBotsMenuInfo.back.focuspic			= ART_BACK1;

	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.background );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.banner );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.arrows );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.up );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.down );
	for( n = 0; n < count; n++ ) {
		Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.bots[n] );
	}
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.Delete );
	Menu_AddItem( &removeBotsMenuInfo.menu, &removeBotsMenuInfo.back );

	removeBotsMenuInfo.baseBotNum = 0;
	removeBotsMenuInfo.selectedBotNum = 0;
	removeBotsMenuInfo.bots[0].color = color_white;
}


/*
=================
UI_RemoveBotsMenu
=================
*/
void UI_RemoveBotsMenu( void ) {
	UI_RemoveBotsMenu_Init();
	UI_PushMenu( &removeBotsMenuInfo.menu );
}

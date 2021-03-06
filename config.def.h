/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#include "fibonacci.c"
#include "grid.c"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 0;        /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
//static const char *fonts[]          = { "monospace:size=12" };
static const char *fonts[]          = { "San Francisco Display:size=12" };
static const char dmenufont[]       = "San Francisco Display:size=13";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_white[]       = "#F3F4F5";
static const char col_blue[]        = "#1C1F25";
static const char col_purple[]      = "#BD93F9";

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_blue, col_gray2 },
	[SchemeSel]  = { col_gray4, col_purple,  col_purple  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
  /* class      instance    title       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           0,         0,        -1 },
  //{ "st",       NULL,       NULL,       0,            0,           1,         1,        -1 },
  { "st",       NULL,       NULL,       0,            0,           1,         0,        -1 },

	//{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",      dwindle },        
  { "HHH",      grid },
};

/* key definitions */
//#define MODKEY  Mod4Mask
//#define MODKEY2 ControlMask
#define MODKEY  ControlMask
#define MODKEY2 Mod4Mask


#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY2,                      KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY2|ShiftMask,            KEY,      tag,            {.ui = 1 << TAG} },
	/*{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, */


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history", "-p", "run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_blue, "-nf", col_white, "-sb", col_purple, "-sf", col_white, NULL };
static const char *flameshotcmd[] = { "flameshot", "gui", "-p", "/home/calebe94/Pictures/Screenshots", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *vol_up[] = {"audio_control", "-i", "5", "-v", NULL};
static const char *vol_down[] = {"audio_control", "-d", "5", "-v", NULL};
static const char *vol_mute[] = { "audio_control", "-mv", NULL};
static const char *keyboard_back_up[] = {"brightness_control", "-k", "-i", "25", "-v", NULL};
static const char *keyboard_back_down[] = {"brightness_control", "-k", "-d", "25", "-v", NULL};
static const char *screen_back_up[] =  {"brightness_control", "-s", "-i", "139", "-v", NULL};
static const char *screen_back_down[] =  {"brightness_control", "-s", "-d", "139", "-v", NULL};
static const char *show_menu[] = {"tmenu", "/home/calebe94/.config/tmenu.yaml", NULL};
static const char *show_dmenu_history[] = {"dmenu_run_history", "-p", "run", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = flameshotcmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd }  },
	{ MODKEY2,                      XK_Return, spawn,          {.v = termcmd }  },
	{ MODKEY,                       XK_b,      togglebar,      {0}              },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY2|ShiftMask,            XK_f,      zoom,           {0} }, // Focus
	{ MODKEY,                       XK_Tab,    view,           {0} },
//	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },
//  { MODKEY,                       XK_s,      setlayout,      {.v = &layouts[3]} },
  { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY2,                      XK_space,  setlayout,      {0} },
	{ MODKEY2|ShiftMask,            XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,                       quit,           {0}                         },
	{ 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = vol_up }              },
	{ 0,                            XF86XK_AudioLowerVolume,    spawn,          {.v = vol_down }            },
	{ 0,                            XF86XK_AudioMute,           spawn,          {.v = vol_mute }            },
  { 0,                            XF86XK_KbdBrightnessUp,     spawn,          {.v = keyboard_back_up }    },
  { 0,                            XF86XK_KbdBrightnessDown,   spawn,          {.v = keyboard_back_down }  },
  { 0,                            XF86XK_MonBrightnessUp,     spawn,          {.v = screen_back_up}       },
  { 0,                            XF86XK_MonBrightnessDown,   spawn,          {.v = screen_back_down}     },
  { 0,                            XF86XK_LaunchB,             spawn,          {.v = show_menu}            },
  { 0,                            XF86XK_LaunchA,             spawn,          {.v = show_dmenu_history }  },
	{ MODKEY,                       XK_q,                       killclient,     {0}                         },
	{ MODKEY2,                      XK_q,                       killclient,     {0}                         },
	{ MODKEY,                       XK_space,                   spawn,          {.v = show_menu }           },
	{ MODKEY|ShiftMask,             XK_space,                   spawn,          {.v = dmenucmd  }           },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


/**
 *
 * Compiz bell plugin
 *
 * bell.c
 *
 * Copyright (c) 2011 Emily Strickland <emily@zubon.org>
 *
 * Authors:
 * Emily Strickland <emily@zubon.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 **/

#include "bell.h"

COMPIZ_PLUGIN_20090315 (bell, BellPluginVTable)

bool
AudibleBell::bell ()
{
    int error;

    if ((error = ca_context_play (mCanberraContext, 0,
                              CA_PROP_EVENT_ID, "bell",
                              CA_PROP_CANBERRA_CACHE_CONTROL, "permanent",
                              NULL)) < 0)
    {
        compLogMessage ("bell", CompLogLevelWarn, "couldn't play bell - %s",
                    ca_strerror (error));
    }

    /* Allow other plugins to handle bell event */
    return false;
}

AudibleBell::AudibleBell (CompScreen *screen) :
    PluginClassHandler <AudibleBell, CompScreen> (screen),
    mCanberraContext (NULL)
{
    int                        error;
    boost::function <bool (CompAction *, CompAction::State, CompOption::Vector &)> bellCallback;

    if ((error = ca_context_create (&mCanberraContext)) < 0)
    {
        compLogMessage ("bell", CompLogLevelWarn, "couldn't initialize canberra - %s",
                        ca_strerror (error));
        setFailed ();
    }
    else
    {
        if ((error = ca_context_change_props (mCanberraContext,
                                      CA_PROP_APPLICATION_NAME,
                                      "Compiz bell plugin",
                                      CA_PROP_APPLICATION_ID,
                                      "org.freedesktop.compiz.Bell",
                                      CA_PROP_WINDOW_X11_SCREEN,
                                      screen->displayString (),
                                      NULL)) < 0)
        {
            compLogMessage ("bell", CompLogLevelWarn, "couldn't register bell handler - %s",
                            ca_strerror (error));
            setFailed ();
        }
        else
        {
            if ((error = ca_context_open (mCanberraContext)) < 0)
            {
                compLogMessage ("bell", CompLogLevelWarn, "couldn't open canberra context - %s",
                                ca_strerror (error));
                setFailed ();
            }
        }
    }

    bellCallback =
    boost::bind (&AudibleBell::bell, this);

    optionSetBellInitiate (bellCallback);
}

AudibleBell::~AudibleBell ()
{
    ca_context_destroy (mCanberraContext);
}

bool
BellPluginVTable::init ()
{
    if (!CompPlugin::checkPluginABI ("core", CORE_ABIVERSION))
         return false;

    return true;
}

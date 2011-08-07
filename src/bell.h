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

#include <core/core.h>
#include <core/pluginclasshandler.h>

#include <canberra.h>

#include "bell_options.h"

class AudibleBell :
    public PluginClassHandler<AudibleBell, CompScreen>,
    public ScreenInterface,
    public BellOptions
{
    public:

        AudibleBell (CompScreen *screen);
        ~AudibleBell ();

	bool bell ();

    private:

        ca_context *mCanberraContext;
};

class BellPluginVTable :
    public CompPlugin::VTableForScreen<AudibleBell>
{
    public:
        bool init ();
};


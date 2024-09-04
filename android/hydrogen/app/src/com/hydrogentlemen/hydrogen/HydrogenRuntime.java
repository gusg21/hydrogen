package com.hydrogentlemen.hydrogen;

import org.libsdl.app.SDLActivity;


public class HydrogenRuntime extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] {
                "HydrogenRuntime", // Seems to be the only thing we need to load :)
//                "SDL2",
                // "SDL2_image",
                // "SDL2_mixer",
                // "SDL2_net",
                // "SDL2_ttf",
//                "main"
        };
    }
}
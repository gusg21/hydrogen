package com.hydrogentlemen.hydrogen;

import org.libsdl.app.SDLActivity;

public class HydrogenRuntimeActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] {
                "HydrogenRuntime"
        };
    }
}

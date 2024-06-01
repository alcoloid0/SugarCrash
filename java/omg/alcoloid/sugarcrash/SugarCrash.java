package omg.alcoloid.sugarcrash;

import java.io.IOException;

import org.bukkit.plugin.java.JavaPlugin;

import cz.adamh.utils.NativeUtils;

public final class SugarCrash extends JavaPlugin {
    private static final String SUGARCRASH_DLL = "SugarCrashJNI.dll";

    @Override
    public void onEnable() {
        String osName = System.getProperty("os.name").toLowerCase();

        if (osName.indexOf("win") >= 0) {
            try {
                NativeUtils.loadLibraryFromJar("/" + SUGARCRASH_DLL);
            } catch (IOException e) {
                e.printStackTrace();
            }

            SugarCrash.sugarCrash();
        }
    }

    private static native void sugarCrash();
}
#-------------------------------------------------------------------------------
# Nebula 3 Mozilla Plugin Registration Script
#
# (c) 2008 Vadim Macagon
#
# Contents are licensed under the Nebula license.
#-------------------------------------------------------------------------------

import sys
import _winreg

#--------------------------------------------------------------------------
def PrintHelp():
    print """\
Usage:
register_plugin.py <"full_path_to_plugin_dll">
"""

#-------------------------------------------------------------------------------
def CreatePluginKey(key):
    hKey = None
    try:
        hKey = _winreg.CreateKey(_winreg.HKEY_LOCAL_MACHINE, key)
    except EnvironmentError, err:
        print std(err)
    
    if not hKey:
        hKey = _winreg.CreateKey(_winreg.HKEY_CURRENT_USER, key)
    
    return hKey

#-------------------------------------------------------------------------------    
def SetPluginKeyValue(key, name, value):
    _winreg.SetValueEx(key, name, 0, _winreg.REG_SZ, value)

#-------------------------------------------------------------------------------
def main():
    #sourceDir = os.getcwd()
    numCmdLineArgs = len(sys.argv)
    if numCmdLineArgs > 1:
        pluginPath = sys.argv[1]
        key = CreatePluginKey(r'SOFTWARE\MozillaPlugins\@macagon.com/Nebula3,version=0.0.1.0')
        if key:
            SetPluginKeyValue(key, r'Description', r'Nebula3 Mozilla Plugin')
            SetPluginKeyValue(key, r'Path', pluginPath)
            SetPluginKeyValue(key, r'ProductName', r'Nebula3')
            SetPluginKeyValue(key, r'Vendor', r'Vadim Macagon')
            SetPluginKeyValue(key, r'Version', r'0.0.1.0')
            _winreg.CloseKey(key)
    else:
        PrintHelp()

#-------------------------------------------------------------------------------
# Entry Point
#-------------------------------------------------------------------------------
if __name__ == '__main__':
    main()

#-------------------------------------------------------------------------------
# EOF
#-------------------------------------------------------------------------------

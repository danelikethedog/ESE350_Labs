#!f:\google~1\school~1\ese350\ese350~1\ese09c~1\digita~1\scripts\python.exe
# EASY-INSTALL-ENTRY-SCRIPT: 'Flask==0.11.dev0','console_scripts','flask'
__requires__ = 'Flask==0.11.dev0'
import sys
from pkg_resources import load_entry_point

if __name__ == '__main__':
    sys.exit(
        load_entry_point('Flask==0.11.dev0', 'console_scripts', 'flask')()
    )

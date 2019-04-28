import os
import shutil
import time
import datetime


    
with open("fileversion.txt","r") as f1:
	content1 = f1.read()
version_arry = content1.split(',')


with open("filebuild.txt","r") as f2:
	content_str = f2.read()

version = version_arry[0]+"." + version_arry[1]+"." + version_arry[2]+"." + content_str

if os.path.isdir("./Output/PDBBackup/V" + version):
    pass
else:
    os.makedirs("./Output/PDBBackup/V" + version)


shutil.copyfile("bin/release/BrowserApp.pdb","./Output/PDBBackup/V" + version + "/BrowserApp.pdb")
shutil.copyfile("bin/release/BrowserApp.exe","./Output/PDBBackup/V" + version + "/BrowserApp.exe")
shutil.copyfile("bin/release_69_3497/BrowserApp.pdb","./Output/PDBBackup/V" + version + "/BrowserApp_69_3497.pdb")
shutil.copyfile("bin/release_69_3497/BrowserApp.exe","./Output/PDBBackup/V" + version + "/BrowserApp_69_3497.exe")

import os

with open("fileversion.txt","r") as f1:
	content1 = f1.read()
version_arry = content1.split(',')


with open("filebuild.txt","r") as f2:
	content_str = f2.read()
	content2 = int(content_str) + 1

with open("filebuild.txt","w") as f2:
	f2.write(str(content2))
	
version = content1 + "," + str(content2)

with open("BrowserApp/fileversion_number.h","w") as f3:
    f3.write("#define IDS_FILEVERSION_VALUE   " + version)
    f3.write("\n")
    f3.write("#define IDS_FILEVERSION_VALUE_V  \"V" + version_arry[0]+"." + version_arry[1]+"." + version_arry[2]+"." + str(content2) + "\"" )
    f3.write("\n")


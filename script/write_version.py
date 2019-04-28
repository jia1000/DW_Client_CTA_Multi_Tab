import os

versionFile = open("../version.txt", "r", encoding="UTF-16")

content = versionFile.readline()


version = content[7:-1]

version = "\"" + version + "\""

versionFile.close()

filesize = os.path.getsize("../BrowserApp/Resource.rc")
resourceFile = open("../BrowserApp/Resource.rc", "r+", encoding="UTF-8")

line = resourceFile.read()

index = line.find("\"FileDescription\",")
fileDiscLen = len("\"FileDescription\",")
index2 = line.find("VALUE \"FileVersion\",")
#index3 = line.find("FILEFLAGSMASK")

if (index != -1 and index2 != -1) :
    fileDesc = line[index+fileDiscLen:index2-2]
    line = line.replace(fileDesc, version)
    print(line)
#replace FILEVERSION
#if (index != -1 and index2 != -1):
#    fvFind = line.rfind(",", index, index2)
#    if (fvFind != -1):
#        minVer = line[fvFind+1:index2-2]
#        line = line.replace(minVer, version, 1)
#        print(line)
    
    
#replace PRODUCTVERSION
#index2 = line.find("PRODUCTVERSION")
#index3 = line.find("FILEFLAGSMASK")
#if (index2 != -1 and index3 != -1) :
#    pvFind = line.rfind(",", index2, index3)
#    if (pvFind != -1):
#        minPv = line[pvFind+1:index3-2]
#        line = line.replace(minPv, version, 1)
#        print(line)

resourceFile.seek(0, 0);

resourceFile.write(line);

resourceFile.close();



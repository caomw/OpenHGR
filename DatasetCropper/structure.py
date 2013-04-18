import os
import random
import shutil

INPUTDIR = "/home/mathieu/Desktop/OpenHGR/data/Dataset2_Cropped"
OUTPUTDIR = "output"
splits = 3

nameAssociation = { 'fred'    : '1',
                    'martin'  : '2',
                    'mathieu' : '3',
                    'rene'    : '4',
                    'thomas'  : '5' }
                    
gestureAssociation = { 'deux'  : '5.two',
                       'index' : '1.index',
                       'paume' : '4.palm',
                       'poing' : '3.fist',
                       'pouce' : '2.thumb' }

illumAssociation = { ''       : '1',
                     'neon'   : '2',
                     'tamise' : '3' }

files = {}
for a in gestureAssociation.values():
    files[a] = []
    
for path, dirs, filenames in os.walk(INPUTDIR):
    for f in filenames:
        split = path.replace(INPUTDIR + '/','').lower().split("/")
        gestureIllum = split[1].split('_')
        
        file = {}
        file['path'] = path + '/' + f
        file['subject'] = nameAssociation[split[0]]
        file['gesture'] = gestureAssociation[gestureIllum[0]]
        file['illum']   = illumAssociation['' if len(gestureIllum) == 1 else gestureIllum[1]]
        
        files[file['gesture']].append( file )

copyCounts = [len(files[g])/splits for g in files]
copyCounts.sort()
copyCount = copyCounts[0]

print 'Copying ' + str(copyCount) + ' pictures/folder'

for gesture in files:
    for i in range(1,splits + 1):
        filesToCopy = random.sample(files[gesture],copyCount)
        files[gesture] = [x for x in files[gesture] if x not in filesToCopy]

        outputDirectory = OUTPUTDIR + '/' + str(i) + '.DataSet/' + gesture
        if not os.path.exists(outputDirectory): 
            os.makedirs(outputDirectory)
        
        j = 1
        for f in filesToCopy:
            outputFile = outputDirectory + '/' + f['illum'] + '.' + f['subject'] + '.' + str(j) + '.jpg'
            shutil.copy2(f['path'], outputFile)
            j += 1
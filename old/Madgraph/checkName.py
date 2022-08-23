import sys
fileName = sys.argv[1]
underscoreLoc = [x for x, myChar in enumerate(fileName) if myChar == '_']
if len(underscoreLoc) == 1 and unicode(fileName[:underscoreLoc[0]],'utf-8').isnumeric():
	print(True)
else:
    if len(underscoreLoc) > 2:	
	underscroeLoc = underscoreLoc[:3]
	mass1 = unicode(fileName[:underscoreLoc[0]],'utf-8')
	mass2 = unicode(fileName[underscoreLoc[0]+1:underscoreLoc[1]], 'utf-8')
	#mass3 = unicode(fileName[underscoreLoc[1]+1:underscoreLoc[2]], 'utf-8')
	status = True
	for mass in [mass1, mass2]:
		if not mass.isnumeric():
			status = False
			break
	print(status)
    elif len(underscoreLoc)==0:
	print("False")
    else:
	underscroeLoc = underscoreLoc[:2]
        mass1 = unicode(fileName[:underscoreLoc[0]],'utf-8')
        mass2 = unicode(fileName[underscoreLoc[0]+1:underscoreLoc[1]], 'utf-8')
        #mass3 = unicode(fileName[underscoreLoc[1]+1:], 'utf-8')
        status = True
        for mass in [mass1, mass2]:
                if not mass.isnumeric():
                        status = False
                        break
        print(status)

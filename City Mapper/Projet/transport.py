#Alexandre Maricic 11803731
#Kévin Chen 11807217
#Nassim Mokeddem 11922864

import csv

# On ouvre en lecture les fichiers "network_combined.csv" et "route.csv" et en écriture le fichier "transport_data.sql"
with open('kuopio/network_combined.csv', 'r') as csv_file, open('kuopio/route.csv', 'r') as route_file, open('data/transport_data.sql', 'w') as sql_file:
    csv1 = csv.reader(csv_file, delimiter = ';')
    csv2 = csv.reader(route_file, delimiter = ';')
    l = {}
    
    #On ne souhaite pas la première ligne de route.csv puisque que c'est les infos
    for line in csv2:
        l[line[0]] = line[1]
    next(csv1)
    
    #Ecrit dans le fichier sql_file les INSERT. Si une ligne a plusieurs routes, alors on les sépare en plusieur
    for line in csv1:
        line[3] = str(round(float(line[3])))
        route = line[5]
        tab = [route.split(",")[i].split(":")[0] for i in range(len(route.split(",")))]
        for i in tab:
            line[4] = l[i]
            stringLine = str(line[:-2])
            sql_file.write("INSERT INTO transport VALUES ("+ stringLine[1:-1] +");\n")
    print('Done')

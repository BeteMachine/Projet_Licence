#Alexandre Maricic 11803731
#Kévin Chen 11807217
#Nassim Mokeddem 11922864


import csv

# On ouvre les fichiers nécessaire pour la création du fichier "walk_data.sql"
with open('kuopio/network_walk.csv','r') as csv_file, open('data/walk_data.sql','w') as sql_file:
    
    # Lecture du csv file
    csv0 = csv.reader(csv_file, delimiter=';')
    
    # On saute la première ligne car on ne veut pas les noms des colonnes
    next(csv0)
    
    # Ecrit dans le fichier sql_file les INSERT
    for line in csv0:
        stringLine = str(line)
        sql_file.write("INSERT INTO walk VALUES ("+ stringLine[1:-1] +");\n")
    print('Done')

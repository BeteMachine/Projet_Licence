# Alexandre Maricic 11803731
# Kévin Chen 11807217
# Nassim Mokeddem 11922864

# On ouvre en lecture le fichier "network_nodes.csv"
f = open("kuopio/network_nodes.csv", 'r')

# On saute la première ligne car on ne veut pas les noms des colonnes
next(f)

# On ouvre en ecriture le fichier "nodes_data.sql"
g = open("data/nodes_data.sql", 'w')

for line in f:
    
    g.write("INSERT INTO nodes VALUES(")
    items = line.rstrip("\n").split(";")
    
    i = 0
    
    # Boucle pour chaque attribut de la liste items
    for item in items :
    
        # Remplace ' par ''
        item = item.replace("'", "''")
        
        if i < 3:
        	g.write("\'%s \'," %item)
        else:
        	g.write("\'%s\'" %item)
        # La position de l'attribut dans la liste et sa valeur
        i = i + 1
    g.write(");\n")
    
f.close()
g.close()

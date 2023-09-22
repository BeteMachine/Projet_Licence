# Alexandre Maricic 11803731
# Kévin Chen 11807217
# Nassim Mokeddem 11922864


import geojson
import sys

# On ouvre en lecture le fichier "routes.geojson"
with open("kuopio/routes.geojson", 'r') as f:
    gj = geojson.load(f)
    sys.stdout = open('kuopio/route.csv', "w")

# On affiche le résulat voulu selon un certain parterne
for line in gj['features']:
    print(f"{line['properties']['route_I']};{line['properties']['route_name']};{line['properties']['route_type']}")

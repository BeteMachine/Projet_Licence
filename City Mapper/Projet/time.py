# Alexandre Maricic 11803731
# Kévin Chen 11807217
# Nassim Mokeddem 11922864

# Le code python n'est pas fonctionnel

import csv
from datetime import datetime

# on ouvre les fichiers csv en lecture
with open('kuopio/network_temporal_week.csv', 'r') as csv1_file, open('kuopio/network_temporal_day.csv', 'r') as csv2_file , open('kuopio/time.csv', 'w') as output_file:
    csv1 = csv.reader(csv1_file, delimiter = ';')
    csv2 = csv.reader(csv2_file, delimiter = ';')
    
    next(csv1)
    for line in csv1:
    	print(datetime.utcfromtimestamp(int(line[2])).strftime('%H:%M:%S'))
        print(datetime.utcfromtimestamp(int(line[3])).strftime('%H:%M:%S'))
    print('Done')

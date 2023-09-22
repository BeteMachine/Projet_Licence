# Alexandre Maricic 11803731
# Kévin Chen 11807217
# Nassim Mokeddem 11922864


import folium, io, json, sys, math, random, os
import psycopg2
from folium.plugins import Draw, MousePosition, MeasureControl
from jinja2 import Template
from branca.element import Element
from PyQt5.QtWebEngineWidgets import QWebEngineView, QWebEnginePage
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

# Filtrage à pied et selon le moyen de transport (train, metro et tram sont inutiles pour notre ville)
bus = 1
walk = 1

class MainWindow(QMainWindow):
    
    def __init__(self):
        super().__init__()

        self.resize(900, 900)

        main = QWidget()
        self.setCentralWidget(main)
        main.setLayout(QVBoxLayout())
        main.setFocusPolicy(Qt.StrongFocus)

        self.tableWidget = QTableWidget()
        self.tableWidget.doubleClicked.connect(self.table_Click)
        self.rows = []

        self.webView = myWebView()

        controls_panel = QHBoxLayout()
        controls_bus = QHBoxLayout()
        controls_walk = QHBoxLayout()
        mysplit = QSplitter(Qt.Vertical)
        mysplit.addWidget(self.tableWidget)
        mysplit.addWidget(self.webView)

        main.layout().addLayout(controls_panel)
        main.layout().addLayout(controls_bus)
        main.layout().addLayout(controls_walk)
        main.layout().addWidget(mysplit)

        _label = QLabel('From: ', self)
        _label.setFixedSize(30,20)
        self.from_box = QComboBox()
        self.from_box.setEditable(True)
        self.from_box.completer().setCompletionMode(QCompleter.PopupCompletion)
        self.from_box.setInsertPolicy(QComboBox.NoInsert)
        controls_panel.addWidget(_label)
        controls_panel.addWidget(self.from_box)

        _label = QLabel('  To: ', self)
        _label.setFixedSize(20,20)
        self.to_box = QComboBox()
        self.to_box.setEditable(True)
        self.to_box.completer().setCompletionMode(QCompleter.PopupCompletion)
        self.to_box.setInsertPolicy(QComboBox.NoInsert)
        controls_panel.addWidget(_label)
        controls_panel.addWidget(self.to_box)

        _label = QLabel('Hops: ', self)
        _label.setFixedSize(20,20)
        self.hop_box = QComboBox()
        self.hop_box.addItems( ['1', '2', '3', '4', '5'] )
        self.hop_box.setCurrentIndex( 2 )
        controls_panel.addWidget(_label)
        controls_panel.addWidget(self.hop_box)

        self.go_button = QPushButton("Go!")
        self.go_button.clicked.connect(self.button_Go)
        controls_panel.addWidget(self.go_button)

        self.clear_button = QPushButton("Clear")
        self.clear_button.clicked.connect(self.button_Clear)
        controls_panel.addWidget(self.clear_button)

        # Pour filtrer les résultats par le moyen de transport (bus) 
        controls_bus.setSpacing(0)
        self.bus = QCheckBox("Bus")
        self.bus.setChecked(True)
        controls_bus.addWidget(self.bus)
        
        # Pour filtrer les résultats à pied 
        controls_walk.setSpacing(0)
        self.walk = QCheckBox("Walk")
        self.walk.setChecked(False)
        controls_walk.addWidget(self.walk)

        self.maptype_box = QComboBox()
        self.maptype_box.addItems(self.webView.maptypes)
        self.maptype_box.currentIndexChanged.connect(self.webView.setMap)
        controls_panel.addWidget(self.maptype_box)

        # Bouton qui permet de trier "from" et "to" le nom des arrêts par ordre alphabetique 
        self.name_button = QPushButton("Order by Name")
        self.name_button.clicked.connect(self.button_Name)
        controls_panel.addWidget(self.name_button)
        
        # Bouton qui permet de trier "from" et "to" l'id des arrêts du plus petit au plus grand
        self.id_button = QPushButton("Order by Id")
        self.id_button.clicked.connect(self.button_Id)
        controls_panel.addWidget(self.id_button)
        
        self.connect_DB()

        self.startingpoint = True
                   
        self.show()
        
    def connect_DB(self):
        self.conn = psycopg2.connect(database="l3info_23", user="l3info_23", host="10.11.11.22", password="L3INFO_23")
        self.cursor = self.conn.cursor()
        self.cursor.execute("""SELECT stop_I, name FROM nodes ORDER BY name""")
        self.conn.commit()
        rows = self.cursor.fetchall()

        for row in rows : 
            self.from_box.addItem(str(row[1])+" Station "+str(row[0]))
            self.to_box.addItem(str(row[1])+" Station "+str(row[0]))
 
        
    def table_Click(self):
        k = 0
        prev_lat = 0
        for col in self.rows[self.tableWidget.currentRow()] :
            if (k % 3) == 0:
                lst = col.split(',')
                lat = float(lst[0])
                lon = float(lst[1]) 

                if prev_lat != 0:
                    self.webView.addSegment( prev_lat, prev_lon, lat, lon )
                prev_lat = lat
                prev_lon = lon

                self.webView.addMarker( lat, lon )
            k = k + 1
        

    def button_Go(self):
        self.tableWidget.clearContents()

        _fromstation = str(self.from_box.currentText()).split(" ")[-1:][0]
        _tostation = str(self.to_box.currentText()).split(" ")[-1:][0]
        _hops = int(self.hop_box.currentText())

        # Ajoute un marqueur lors du lancement de la requete au point de départ
        self.cursor.execute(""f" SELECT lat, lon FROM nodes WHERE stop_I = {_fromstation} """)
        self.conn.commit()
        f = self.cursor.fetchall()
        self.webView.addMarker(f[0][0], f[0][1])
	
	# Ajoute un marqueur lors du lancement de la requete au point d'arrivé
        self.cursor.execute(""f" SELECT lat, lon FROM nodes WHERE stop_I = {_tostation} """)
        self.conn.commit()
        t = self.cursor.fetchall()
        self.webView.addMarker(t[0][0], t[0][1])
        
        self.rows = []
        
        # Requete pour avoir le chemin à pied entre 2 stations
        if self.walk.isChecked() :
            self.cursor.execute(""f"\
            WITH walk_0(from_stop_i, name_from, to_stop_i, name_stop, d_walk) as(\
               SELECT from_stop_i, node1.name, to_stop_i, node2.name, d_walk\
               FROM walk, nodes as node1, nodes as node2\
               WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
            SELECT name_from,  name_stop,(CONCAT(d_walk, 'm')) as d_walk_tot\
            FROM walk_0\
            WHERE from_stop_i = $${_fromstation}$$ AND to_stop_i = $${_tostation}$$\
            ORDER BY d_walk_tot""")
            self.conn.commit()
            self.rows += self.cursor.fetchall()
           


        # Requete pour avoir les chemins entre 2 stations avec un seul moyen de transport
        if _hops >= 1 and self.bus.isChecked() :
           self.cursor.execute(""f"\
           WITH transport_1(from_stop_i, name_from, to_stop_i, name_stop, route_type, duration_avg) as(\
                SELECT from_stop_i, node1.name, to_stop_i, node2.name, route_type, duration_avg\
                FROM transport, nodes as node1, nodes as node2\
                WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
           SELECT name_from, route_type, name_stop,(CONCAT(duration_avg, 's')) as duration_avg_tot\
           FROM transport_1\
           WHERE from_stop_i = $${_fromstation}$$ AND to_stop_i = $${_tostation}$$\
           ORDER BY duration_avg_tot""")
           self.conn.commit()
           self.rows += self.cursor.fetchall()
        
      
        
        # Requete pour avoir les chemins entre 2 stations en utilisant deux moyens de transports
        if _hops >= 2 and self.bus.isChecked() :
           self.cursor.execute(""f"\
           WITH transport_2(from_stop_i, name_from, to_stop_i, name_stop, route_type, duration_avg) as(\
                SELECT from_stop_i, node1.name, to_stop_i, node2.name, route_type,duration_avg\
                FROM transport,nodes as node1, nodes as node2\
                WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
           SELECT A.name_from, A.route_type, A.name_stop, B.route_type, B.name_stop, (CONCAT(A.duration_avg + B.duration_avg, 's')) as duration_avg_tot\
           FROM transport_2 as A, transport_2 as B\
           WHERE A.from_stop_i = $${_fromstation}$$ AND B.to_stop_i = $${_tostation}$$ AND A.to_stop_i = B.from_stop_i\
           ORDER BY duration_avg_tot""")
           self.conn.commit()
           self.rows += self.cursor.fetchall()



        if _hops >= 3 and self.bus.isChecked() : 
           self.cursor.execute(""f"\
           WITH transport_3(from_stop_i, name_from, to_stop_i, name_stop, route_type, duration_avg) as(\
                SELECT from_stop_i, node1.name, to_stop_i, node2.name, route_type, duration_avg\
                FROM transport, nodes as node1, nodes as node2\
                WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
           SELECT A.name_from, A.route_type, A.name_stop, B.route_type, B.name_stop, C.route_type, C.name_stop, (CONCAT(A.duration_avg + B.duration_avg + C.duration_avg, 's')) as duration_avg_tot\
           FROM transport_3 as A, transport_3 as B,transport_3 as C\
           WHERE A.from_stop_i = $${_fromstation}$$ AND C.to_stop_i = $${_tostation}$$ AND A.to_stop_i = B.from_stop_i AND B.to_stop_i = C.from_stop_i AND B.to_stop_i <> A.from_stop_i AND B.to_stop_i <> C.to_stop_i\
           ORDER BY duration_avg_tot""")
           self.conn.commit()
           self.rows += self.cursor.fetchall()



        if _hops >= 4 and self.bus.isChecked() : 
           self.cursor.execute(""f"\
            WITH transport_4(from_stop_i, name_from, to_stop_i, name_stop, route_type, duration_avg) as(\
                SELECT from_stop_i, node1.name, to_stop_i, node2.name, route_type, duration_avg\
                FROM transport, nodes as node1, nodes as node2\
                WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
           SELECT A.name_from, A.route_type, A.name_stop, B.route_type, B.name_stop, C.route_type, C.name_stop, D.route_type, D.name_stop, (CONCAT(A.duration_avg + B.duration_avg + C.duration_avg + D.duration_avg, 's')) as duration_avg_tot\
           FROM transport_4 as A, transport_4 as B,transport_4 as C, transport_4 as D\
           WHERE A.from_stop_i = $${_fromstation}$$ AND D.to_stop_i = $${_tostation}$$ AND A.to_stop_i = B.from_stop_i AND B.to_stop_i = C.from_stop_i AND C.to_stop_i = D.from_stop_i AND B.to_stop_i <> A.from_stop_i AND B.to_stop_i <> C.to_stop_i\
           ORDER BY duration_avg_tot""")
           self.conn.commit()
           self.rows += self.cursor.fetchall()
           
           
           
           if _hops >= 5 and self.bus.isChecked() : 
              self.cursor.execute(""f"\
           WITH transport_5(from_stop_i, name_from, to_stop_i, name_stop, route_type, duration_avg) as(\
                SELECT from_stop_i, node1.name, to_stop_i, node2.name, route_type, duration_avg\
                FROM transport, nodes as node1, nodes as node2\
                WHERE from_stop_i = node1.stop_I and to_stop_i = node2.stop_I)\
           SELECT A.name_from, A.route_type, A.name_stop, B.route_type, B.name_stop, C.route_type, C.name_stop, D.route_type, D.name_stop, E.route_type, E.name_stop, (CONCAT(A.duration_avg + B.duration_avg + C.duration_avg + D.duration_avg + E.duration_avg, 's')) as duration_avg_tot\
           FROM transport_5 as A, transport_5 as B,transport_5 as C, transport_5 as D, transport_5 as E\
           WHERE A.from_stop_i = $${_fromstation}$$ AND E.to_stop_i = $${_tostation}$$ AND A.to_stop_i = B.from_stop_i AND B.to_stop_i = C.from_stop_i AND C.to_stop_i = D.from_stop_i AND D.to_stop_i = E.from_stop_i AND B.to_stop_i <> A.from_stop_i AND B.to_stop_i <> C.to_stop_i AND A.from_stop_i <> C.to_stop_i AND B.to_stop_i <> D.from_stop_i AND A.to_stop_i <> D.from_stop_i\
           ORDER BY duration_avg_tot""")
           self.conn.commit()
           self.rows += self.cursor.fetchall()
          


        if len(self.rows) == 0 : 
            self.tableWidget.setRowCount(0)
            self.tableWidget.setColumnCount(0)
            return

        # On initialise un tableau
        numrows = len(self.rows)
        numcols = len(self.rows[-1])
        self.tableWidget.setRowCount(numrows)
        self.tableWidget.setColumnCount(numcols)
        
        # Remplissage du tableau avec les données
        i = 0
        for row in self.rows:
            j = 0
            for col in row:
                self.tableWidget.setItem(i, j, QTableWidgetItem(str(col)))
                j = j + 1
            i = i + 1
            
        header = self.tableWidget.horizontalHeader()
        j = 0
        while j < numcols :
            header.setSectionResizeMode(j, QHeaderView.ResizeToContents)
            j = j + 1
        
        self.update()
        
        
    def button_Clear(self):
        self.webView.clearMap(self.maptype_box.currentIndex())
        self.startingpoint = True
        self.update()


    def mouseClick(self, lat_f, lng_f):
        self.webView.addPointMarker(lat_f, lng_f)

        print(f"Clicked on: latitude {lat_f}, longitude {lng_f}")
        self.cursor.execute(""f" WITH mytable (distance, nom)\
        AS (SELECT ( ABS(lat-{lat_f}) + ABS(lon-{lng_f}) ), name FROM nodes)\
        SELECT A.nom FROM mytable as A WHERE A.distance <=  (SELECT min(B.distance)\
        FROM mytable as B)""")
        
        self.conn.commit()
        rows = self.cursor.fetchall()
        #print('Closest STATION is: ', rows[0][0])
        if self.startingpoint :
            self.from_box.setCurrentIndex(self.from_box.findText(rows[0][0], Qt.MatchStartsWith))
        else :
            self.to_box.setCurrentIndex(self.to_box.findText(rows[0][0], Qt.MatchStartsWith))
        self.startingpoint = not self.startingpoint


    def button_Name(self):
        self.from_box.clear()
        self.to_box.clear()
        self.cursor = self.conn.cursor()
        self.cursor.execute("""SELECT stop_I, name FROM nodes ORDER BY name""")
        self.conn.commit()
        rows = self.cursor.fetchall()

        for row in rows : 
            self.from_box.addItem(str(row[1])+" Station "+str(row[0]))
            self.to_box.addItem(str(row[1])+" Station "+str(row[0]))
    
    def button_Id(self):
        self.from_box.clear()
        self.to_box.clear()
        self.cursor = self.conn.cursor()
        self.cursor.execute("""SELECT stop_I, name FROM nodes ORDER BY stop_I""")
        self.conn.commit()
        rows = self.cursor.fetchall()

        for row in rows : 
            self.from_box.addItem(str(row[1])+" Station "+str(row[0]))
            self.to_box.addItem(str(row[1])+" Station "+str(row[0]))



class myWebView (QWebEngineView):
    def __init__(self):
        super().__init__()

        self.maptypes = ["OpenStreetMap", "Stamen Terrain", "stamentoner", "cartodbpositron"]
        self.setMap(0)


    def add_customjs(self, map_object):
        my_js = f"""{map_object.get_name()}.on("click",
                 function (e) {{
                    var data = `{{"coordinates": ${{JSON.stringify(e.latlng)}}}}`;
                    console.log(data)}}); """
        e = Element(my_js)
        html = map_object.get_root()
        html.script.get_root().render()
        html.script._children[e.get_name()] = e

        return map_object


    def handleClick(self, msg):
        data = json.loads(msg)
        lat = data['coordinates']['lat']
        lng = data['coordinates']['lng']


        window.mouseClick(lat, lng)


    def addSegment(self, lat1, lng1, lat2, lng2):
        js = Template(
        """
        L.polyline(
            [ [{{latitude1}}, {{longitude1}}], [{{latitude2}}, {{longitude2}}] ], {
                "color": "red",
                "opacity": 1.0,
                "weight": 4,
                "line_cap": "butt"
            }
        ).addTo({{map}});
        """
        ).render(map=self.mymap.get_name(), latitude1=lat1, longitude1=lng1, latitude2=lat2, longitude2=lng2 )

        self.page().runJavaScript(js)


    def addMarker(self, lat, lng):
        js = Template(
        """
        L.marker([{{latitude}}, {{longitude}}] ).addTo({{map}});
        L.circleMarker(
            [{{latitude}}, {{longitude}}], {
                "bubblingMouseEvents": true,
                "color": "#3388ff",
                "popup": "hello",
                "dashArray": null,
                "dashOffset": null,
                "fill": false,
                "fillColor": "#3388ff",
                "fillOpacity": 0.2,
                "fillRule": "evenodd",
                "lineCap": "round",
                "lineJoin": "round",
                "opacity": 1.0,
                "radius": 2,
                "stroke": true,
                "weight": 5
            }
        ).addTo({{map}});
        """
        ).render(map=self.mymap.get_name(), latitude=lat, longitude=lng)
        self.page().runJavaScript(js)


    def addPointMarker(self, lat, lng):
        js = Template(
        """
        L.circleMarker(
            [{{latitude}}, {{longitude}}], {
                "bubblingMouseEvents": true,
                "color": 'green',
                "popup": "hello",
                "dashArray": null,
                "dashOffset": null,
                "fill": false,
                "fillColor": 'green',
                "fillOpacity": 0.2,
                "fillRule": "evenodd",
                "lineCap": "round",
                "lineJoin": "round",
                "opacity": 1.0,
                "radius": 2,
                "stroke": true,
                "weight": 5
            }
        ).addTo({{map}});
        """
        ).render(map=self.mymap.get_name(), latitude=lat, longitude=lng)
        self.page().runJavaScript(js)


    def setMap (self, i):
        self.mymap = folium.Map(location=[62.89238, 27.67703], tiles=self.maptypes[i], zoom_start=12, prefer_canvas=True)

        self.mymap = self.add_customjs(self.mymap)

        page = WebEnginePage(self)
        self.setPage(page)

        data = io.BytesIO()
        self.mymap.save(data, close_file=False)

        self.setHtml(data.getvalue().decode())


    def clearMap(self, index):
        self.setMap(index)



class WebEnginePage(QWebEnginePage):
    def __init__(self, parent):
        super().__init__(parent)
        self.parent = parent


    def javaScriptConsoleMessage(self, level, msg, line, sourceID):
        #print(msg)
        if 'coordinates' in msg:
            self.parent.handleClick(msg)
			
if __name__ == '__main__':
    app = QApplication(sys.argv) 
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())

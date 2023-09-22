<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:ex="http://example.org/xmlns/2002/document"
                xmlns:xlink="http://www.w3.org/1999/xlink"
                exclude-result-prefixes="ex xlink">
  <xsl:output method="html" indent="yes"/>
  <xsl:template match="/ex:listeCV">
    <html>
      <head>
        <title>Liste des CVs</title>
      </head>
      <body>
        <h1>Liste des CVs</h1> 
        <ul>
          <xsl:for-each select="ex:ancre">
            <li>
              <a href="{@xlink:href}"><xsl:value-of select="."/></a>
            </li>
          </xsl:for-each>
        </ul>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>

<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		xmlns:fo="http://www.w3.org/1999/XSL/Format" 
        xmlns:xlink="http://www.w3.org/1999/xlink" 
        xmlns:doc="http://example.org/xmlns/2002/document"
		version="1.0">
  <xsl:output method="xml" encoding="UTF-8"/>
  <xsl:template match="/doc:listeCV">
    <fo:root>
      <fo:layout-master-set>
        <fo:simple-page-master master-name="page" margin="20pt">
          <fo:region-body margin-top="16pt" margin-bottom="2pt"/>
        </fo:simple-page-master>
      </fo:layout-master-set>
      <fo:page-sequence master-reference="page">
        <fo:flow flow-name="xsl-region-body">
          <fo:block font-size="16pt" font-weight="bold" text-align="center" margin-bottom="10pt">List of CVs</fo:block>
          <xsl:for-each select="doc:ancre">
            <fo:block font-size="12pt" font-weight="normal" text-align="left" margin-bottom="5pt">
              <fo:basic-link external-destination="{@xlink:href}">
                <xsl:value-of select="."/>
              </fo:basic-link>
            </fo:block>
          </xsl:for-each>
        </fo:flow>
      </fo:page-sequence>
    </fo:root>
  </xsl:template>
</xsl:stylesheet>

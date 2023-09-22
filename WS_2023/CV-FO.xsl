<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <!-- Modifiez le template ci-dessous pour personnaliser la conversion en XSL-FO -->
  <xsl:template match="/">
    <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
      <fo:layout-master-set>
        <fo:simple-page-master master-name="cv-page" page-width="8.5in" page-height="11in" margin-top="0.5in" margin-bottom="0.5in" margin-left="0.5in" margin-right="0.5in">
          <fo:region-body/>
        </fo:simple-page-master>
      </fo:layout-master-set>
      <fo:page-sequence master-reference="cv-page">
        <fo:flow flow-name="xsl-region-body">
          <fo:block font-size="16pt" font-weight="bold" text-align="center">
            <xsl:value-of select="CV/header/name"/>
          </fo:block>
          <fo:block font-size="12pt" font-style="italic" text-align="center" color="gray">
            Email: <xsl:value-of select="CV/header/email"/>
          </fo:block>
          <xsl:if test="CV/header/address">
          <fo:block font-size="12pt" font-style="italic" text-align="center" color="gray">
            <xsl:text>Adresse: </xsl:text>
            <xsl:value-of select="CV/header/address"/>
          </fo:block>
          </xsl:if>
          
          <fo:block font-size="16pt" font-weight="bold" margin-top="10pt">
            Résumé
          </fo:block>
          <fo:block font-size="14pt" font-weight="bold" margin-top="10pt" color="blue" margin-left="30px">
            <xsl:value-of select="CV/summary/summary_headline"/>
          </fo:block>
          <fo:block font-size="12pt" margin-top="6pt" color="gray" margin-left="60px">
            <xsl:value-of select="CV/summary/summary_description"/>
          </fo:block>
          
          <fo:block font-size="16pt" font-weight="bold" margin-top="10pt">
            Expérience
          </fo:block>
          <xsl:for-each select="CV/experience/job">
            <fo:block font-size="14pt" font-weight="bold" margin-top="6pt" margin-left="30px" color="blue">
              <xsl:value-of select="job_title"/>
            </fo:block>
            <fo:block font-size="12pt" margin-top="4pt" color="gray" margin-left="60px">
              <xsl:text>Entreprise: </xsl:text>
              <xsl:value-of select="job_company"/>
            </fo:block>
            <fo:block font-size="12pt" color="gray" margin-left="60px">
              <xsl:text>Dates: </xsl:text>
              <xsl:value-of select="job_dates"/>
            </fo:block>
            <fo:block font-size="12pt" color="gray" margin-left="60px">
              <xsl:text>Description: </xsl:text>
              <xsl:apply-templates select="job_description"/>
            </fo:block>
          </xsl:for-each>
          
          <fo:block font-size="16pt" font-weight="bold" margin-top="10pt">
            Formation
          </fo:block>
          <xsl:for-each select="CV/education/degree">
            <fo:block font-size="14pt" font-weight="bold" margin-top="6pt" color="blue" margin-left="30px">
              <xsl:value-of select="degree_title"/>
            </fo:block>
            <fo:block font-size="12pt" margin-top="6pt" color="gray" margin-left="60px">
              <xsl:text>Institution: </xsl:text>
              <xsl:value-of select="degree_institution"/>
            </fo:block>
            <xsl:if test="degree_dates">
            <fo:block font-size="12pt" color="gray" margin-left="60px">
              <xsl:text>Dates: </xsl:text>
              <xsl:value-of select="degree_dates"/>
            </fo:block>
            </xsl:if>
            <xsl:if test="degree_mention">
            <fo:block font-size="12pt" color="gray" margin-left="60px">
              <xsl:text>Mention: </xsl:text>
              <xsl:value-of select="degree_mention"/>
            </fo:block>
            </xsl:if>
          </xsl:for-each>

          <fo:block-container>
            <fo:block>
              <fo:table>
                <fo:table-column column-width="33%"/>
                <fo:table-column column-width="34%"/>
                <fo:table-column column-width="33%"/>
                <fo:table-body>
                  <fo:table-row>
                    <fo:table-cell>
                    <fo:block font-size="16pt" font-weight="bold" margin-top="10pt" >
                        Compétences
                      </fo:block>
                      <fo:block font-size="12pt" margin-top="6pt" color="red">
                        <fo:list-block>
                          <xsl:for-each select="CV/skills/skill">
                            <fo:list-item>
                              <fo:list-item-label end-indent="label-end()" color="red" margin-left="10px">
                                <fo:block>&#8226;</fo:block>
                              </fo:list-item-label>
                              <fo:list-item-body start-indent="body-start()">
                                <fo:block>
                                  <xsl:value-of select="."/>
                                </fo:block>
                              </fo:list-item-body>
                            </fo:list-item>
                          </xsl:for-each>
                        </fo:list-block>
                      </fo:block>
                    </fo:table-cell>

                    <fo:table-cell>
                      <fo:block></fo:block>
                      <xsl:if test="CV/langues">
                      <fo:block font-size="16pt" font-weight="bold" margin-top="10pt">
                        Langues
                      </fo:block>
                      <fo:block>
                        <xsl:if test="CV/langues/langue">
                        <xsl:for-each select="CV/langues/langue">
                          <fo:block font-size="12pt" color="gray" margin-left="10px">
                            <xsl:value-of select="."/>
                          </fo:block> 
                        </xsl:for-each>
                      </xsl:if>
                    </fo:block>
                    </xsl:if>
                    </fo:table-cell>

                    <fo:table-cell>
                      <fo:block></fo:block>
                      <xsl:if test="CV/loisir">
                      <fo:block font-size="14pt" font-weight="bold" margin-top="10pt">
                        Loisirs
                      </fo:block>
                      <fo:block font-size="12pt" margin-top="6pt" color="orange">
                        <fo:list-block>
                          <xsl:for-each select="CV/loisir/activite">
                            <fo:list-item>
                              <fo:list-item-label end-indent="label-end()" color="orange" margin-left="10px">
                                <fo:block>&#8226;</fo:block>
                              </fo:list-item-label>
                              <fo:list-item-body start-indent="body-start()">
                                <fo:block>
                                  <xsl:value-of select="."/>
                                </fo:block>
                              </fo:list-item-body>
                            </fo:list-item>
                          </xsl:for-each>
                        </fo:list-block>
                      </fo:block>
                      </xsl:if>
                    </fo:table-cell>

                  </fo:table-row>
                </fo:table-body>
              </fo:table>
            </fo:block>
          </fo:block-container>
          
        </fo:flow>
      </fo:page-sequence>
    </fo:root>
  </xsl:template>
</xsl:stylesheet>
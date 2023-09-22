<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
		version="1.0">
  <xsl:output method="html" encoding="UTF-8"/>
  <!-- HTML output -->
  <xsl:template match="/">
    <html>
      <head>
      	<title>CV - <xsl:value-of select="CV/header/name"/></title>
				<style>
	  			p {
						margin-left : 60px;
						color: dimgray;
					}
          .h {
            margin-left:0px;
            font-style : italic;
            text-align: center;
          }
          t {
            color: black;
          }
					h1 {
						text-align: center;
					}
					h3 {
						margin-left: 30px;
						color: blue;
					}
          .content{
            display: flex;
          }
          .left{
            flex-basis: 30%;
            padding-right: 20px;
          }
          .right{
            flex-basis: 70%;
          }
					ul {
						margin-left: 10px;
						color: red;
					}
          .g{
             color:lightcyan;
          }
					.desc{
						margin-left:80px;
					}
          body{
            font-family:sans-serif;
            background-color: silver;
          }
				</style>
      </head>
      <body>
      	<h1><xsl:value-of select="CV/header/name"/></h1>
        <p class="h"><t>Email : </t><xsl:value-of select="CV/header/email"/></p>    
				<xsl:if test="CV/header/address">    
      		<p class="h"><t>Adresse : </t><xsl:value-of select="CV/header/address"/></p>
        </xsl:if>
        <p class="h"><a href="http://www.bdpedia.fr">Bdpedia</a></p>
        <h2>Résumé</h2>
        <h3><xsl:value-of select="CV/summary/summary_headline"/></h3>
        <p><xsl:value-of select="CV/summary/summary_description"/></p>
        
        <h2>Expérience</h2>
        <xsl:for-each select="CV/experience/job">
          <h3><xsl:value-of select="job_title"/></h3>
          <p>Entreprise : <xsl:value-of select="job_company"/></p>
          <p>Dates : <xsl:value-of select="job_dates"/></p>
          <p>Description :</p>
					<xsl:for-each select="job_description">
						<p class="desc"><xsl:value-of select="."/></p>
					</xsl:for-each>
        </xsl:for-each>
        
        <h2>Formation</h2>
        <xsl:for-each select="CV/education/degree">
          <h3><xsl:value-of select="degree_title"/></h3>
          <p>Institution : <xsl:value-of select="degree_institution"/></p>
          <xsl:if test="degree_dates">
						<p>Dates : <xsl:value-of select="degree_dates"/></p>
					</xsl:if>
					<xsl:if test="degree_mention">
          	<p >Mention : <xsl:value-of select="degree_mention"/></p>
					</xsl:if>
        </xsl:for-each>

        <xsl:if test="CV/langues">
          <h2>Langues</h2>
          <xsl:for-each select="CV/langues/langue">
            <p><xsl:value-of select="."/></p>
          </xsl:for-each>
        </xsl:if>
        <div class="content">
          <div class="left">
            <h2>Compétences</h2>
            <ul>
              <xsl:for-each select="CV/skills/skill">
                <li><xsl:value-of select="."/></li>
              </xsl:for-each>
            </ul>
          </div>
          <div class="rigth">
            <xsl:if test="CV/loisir">
            <h2>Loisirs</h2>
            <ul class="g">
              <xsl:for-each select="CV/loisir/activite">
                <li><xsl:value-of select="."/></li>
              </xsl:for-each>
            </ul>
            </xsl:if>
          </div>
        </div>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>

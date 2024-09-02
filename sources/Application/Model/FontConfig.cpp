#include "FontConfig.h"
#include "System/Console/Trace.h"
#include "Externals/TinyXML/tinyxml.h"
#include "Application/Model/Config.h"

unsigned char font[8*8*128]= {
	#include "Resources/font.h"
};

const unsigned char embeddedFontBank[FONT_TYPE_COUNT][8*8*128]= {
	{//0 original
		#include "Resources/original.txt"
	},
	{//1 digital
		#include "Resources/digital.txt"
	},
	{//2 monster
		#include "Resources/monster.txt"
	}
};



void FontConfig(){

const char *fontType=Config::GetInstance()->GetValue("FONTTYPE") ;

Trace::Log("CUSTOM_FONT","187") ;
	if (fontType)
	{
		Trace::Log("CUSTOM_FONT","190") ;
		if(strcmp(fontType, "CUSTOM") == 0 || strcmp(fontType, "custom") == 0){
			Trace::Log("CUSTOM_FONT","192") ;
			Path path("bin:custom_font.xml") ;
			TiXmlDocument document(path.GetPath());
			Trace::Log("CUSTOM_FONT","195") ;
			bool loadOkay = document.LoadFile();
			Trace::Log("CUSTOM_FONT","197") ;
			if (!loadOkay) {
				Trace::Log("CUSTOM_FONT","No (bad?) custom_font file") ;
			}
			else{
				Trace::Log("CUSTOM_FONT","202") ;
				TiXmlNode* rootnode = 0;
				rootnode = document.FirstChild( "FONT" );
				if (!rootnode)  {
					Trace::Error("No master node for custom_font") ;
				}
				else{
					Trace::Log("CUSTOM_FONT","209") ;
					TiXmlElement *rootelement = rootnode->ToElement();
					TiXmlNode *node = rootelement->FirstChildElement() ;
					if (node) {
						Trace::Log("CUSTOM_FONT","213") ;
						TiXmlElement *element = node->ToElement();
						while (element) {
							const char *elem=element->Value() ; // sould be DATA but we don't really care
							const char *font_data=element->Attribute("value") ;
							if (font_data) {
								int i = 0, j = 0;
								while (font_data[i] != '\0') {
									if (font_data[i] == '1') {
										font[j++] = 1;
									} else if (font_data[i] == '0') {
										font[j++] = 0;
									}
									i++;
								}
							}
							element = element->NextSiblingElement();
						}
					}
				}
			}
		}
		else if(0 <= atoi(fontType) && atoi(fontType) < FONT_TYPE_COUNT){
            Trace::Log("CUSTOM_FONT","60") ;
            
			for(int i=0;i<8*8*(FONT_COUNT+1);i++)
			{
				font[i]=embeddedFontBank[atoi(fontType)][i];
			}
            Trace::Log("CUSTOM_FONT","213-%d",font[11]) ;
		}
	}
}
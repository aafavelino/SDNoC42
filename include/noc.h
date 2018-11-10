//
// Created by Adelino, Raul and Yuri on 02/11/18.
//

#ifndef _NOC_H_
#define _NOC_H_ 

#include <iostream>
#include <vector>
#include <tuple>
#include "../constantes/constantes.h"
#include "roteador.h"

using namespace std;

SC_MODULE (noc) {

	roteador *network[ALTURA_REDE][LARGURA_REDE];
	int contador_nomes = 0;
	int contador_canais_TERRA = 0;

	// sc_signal< sc_uint <32> > canais_de_comunicacao[1445];
	sc_signal< sc_uint <32> > canais_de_comunicacao_TERRA[((ALTURA_REDE+LARGURA_REDE)*2)];

	SC_CTOR(noc) {
		const char* roteadores_nomes[]={
			"roteador_01" , "roteador_02" , "roteador_03" , "roteador_04" , "roteador_05" , "roteador_06" , "roteador_07" , "roteador_08" , "roteador_09" , "roteador_10" ,
			"roteador_11" , "roteador_12" , "roteador_13" , "roteador_14" , "roteador_15" , "roteador_16" , "roteador_17" , "roteador_18" , "roteador_19" , "roteador_20" ,
			"roteador_21" , "roteador_22" , "roteador_23" , "roteador_24" , "roteador_25" , "roteador_26" , "roteador_27" , "roteador_28" , "roteador_29" , "roteador_30" ,
			"roteador_31" , "roteador_32" , "roteador_33" , "roteador_34" , "roteador_35" , "roteador_36" , "roteador_37" , "roteador_38" , "roteador_39" , "roteador_40" ,
			"roteador_41" , "roteador_42" , "roteador_43" , "roteador_44" , "roteador_45" , "roteador_46" , "roteador_47" , "roteador_48" , "roteador_49" , "roteador_50" ,
			"roteador_51" , "roteador_52" , "roteador_53" , "roteador_54" , "roteador_55" , "roteador_56" , "roteador_57" , "roteador_58" , "roteador_59" , "roteador_60" ,
			"roteador_61" , "roteador_62" , "roteador_63" , "roteador_64" , "roteador_65" , "roteador_66" , "roteador_67" , "roteador_68" , "roteador_69" , "roteador_70" ,
			"roteador_71" , "roteador_72" , "roteador_73" , "roteador_74" , "roteador_75" , "roteador_76" , "roteador_77" , "roteador_78" , "roteador_79" , "roteador_80" ,
			"roteador_81" , "roteador_82" , "roteador_83" , "roteador_84" , "roteador_85" , "roteador_86" , "roteador_87" , "roteador_88" , "roteador_89" , "roteador_90" ,
			"roteador_91" , "roteador_92" , "roteador_93" , "roteador_94" , "roteador_95" , "roteador_96" , "roteador_97" , "roteador_98" , "roteador_99" , "roteador_100",
			"roteador_101", "roteador_102", "roteador_103", "roteador_104", "roteador_105", "roteador_106", "roteador_107", "roteador_108", "roteador_109", "roteador_110",
			"roteador_111", "roteador_112", "roteador_113", "roteador_114", "roteador_115", "roteador_116", "roteador_117", "roteador_118", "roteador_119", "roteador_120",
			"roteador_121", "roteador_122", "roteador_123", "roteador_124", "roteador_125", "roteador_126", "roteador_127", "roteador_128", "roteador_129", "roteador_130",
			"roteador_131", "roteador_132", "roteador_133", "roteador_134", "roteador_135", "roteador_136", "roteador_137", "roteador_138", "roteador_139", "roteador_140",
			"roteador_141", "roteador_142", "roteador_143", "roteador_144", "roteador_145", "roteador_146", "roteador_147", "roteador_148", "roteador_149", "roteador_150",
			"roteador_151", "roteador_152", "roteador_153", "roteador_154", "roteador_155", "roteador_156", "roteador_157", "roteador_158", "roteador_159", "roteador_160",
			"roteador_161", "roteador_162", "roteador_163", "roteador_164", "roteador_165", "roteador_166", "roteador_167", "roteador_168", "roteador_169", "roteador_170",
			"roteador_171", "roteador_172", "roteador_173", "roteador_174", "roteador_175", "roteador_176", "roteador_177", "roteador_178", "roteador_179", "roteador_180",
			"roteador_181", "roteador_182", "roteador_183", "roteador_184", "roteador_185", "roteador_186", "roteador_187", "roteador_188", "roteador_189", "roteador_190",
			"roteador_191", "roteador_192", "roteador_193", "roteador_194", "roteador_195", "roteador_196", "roteador_197", "roteador_198", "roteador_199", "roteador_200",
			"roteador_201", "roteador_202", "roteador_203", "roteador_204", "roteador_205", "roteador_206", "roteador_207", "roteador_208", "roteador_209", "roteador_210",
			"roteador_211", "roteador_212", "roteador_213", "roteador_214", "roteador_215", "roteador_216", "roteador_217", "roteador_218", "roteador_219", "roteador_220",
			"roteador_221", "roteador_222", "roteador_223", "roteador_224", "roteador_225", "roteador_226", "roteador_227", "roteador_228", "roteador_229", "roteador_230",
			"roteador_231", "roteador_232", "roteador_233", "roteador_234", "roteador_235", "roteador_236", "roteador_237", "roteador_238", "roteador_239", "roteador_240",
			"roteador_241", "roteador_242", "roteador_243", "roteador_244", "roteador_245", "roteador_246", "roteador_247", "roteador_248", "roteador_249", "roteador_250",
			"roteador_251", "roteador_252", "roteador_253", "roteador_254", "roteador_255", "roteador_256", "roteador_257", "roteador_258", "roteador_259", "roteador_260",
			"roteador_261", "roteador_262", "roteador_263", "roteador_264", "roteador_265", "roteador_266", "roteador_267", "roteador_268", "roteador_269", "roteador_270",
			"roteador_271", "roteador_272", "roteador_273", "roteador_274", "roteador_275", "roteador_276", "roteador_277", "roteador_278", "roteador_279", "roteador_280",
			"roteador_281", "roteador_282", "roteador_283", "roteador_284", "roteador_285", "roteador_286", "roteador_287", "roteador_288", "roteador_289", "roteador_290",
			"roteador_291", "roteador_292", "roteador_293", "roteador_294", "roteador_295", "roteador_296", "roteador_297", "roteador_298", "roteador_299", "roteador_300",
			"roteador_301", "roteador_302", "roteador_303", "roteador_304", "roteador_305", "roteador_306", "roteador_307", "roteador_308", "roteador_309", "roteador_310",
			"roteador_311", "roteador_312", "roteador_313", "roteador_314", "roteador_315", "roteador_316", "roteador_317", "roteador_318", "roteador_319", "roteador_320",
			"roteador_321", "roteador_322", "roteador_323", "roteador_324", "roteador_325", "roteador_326", "roteador_327", "roteador_328", "roteador_329", "roteador_330"
		};
		
		for (int i = 0; i < ALTURA_REDE; ++i){
			for (int j = 0; j < LARGURA_REDE; ++j){
				cout << i << " " << j << " "<< roteadores_nomes[contador_nomes] << endl;
				network[i][j] = new roteador(roteadores_nomes[contador_nomes]);
				contador_nomes++;
			}
		}

		// Ligando sinais entre os roteadores	
		for (int x = 0; x < ALTURA_REDE; ++x)
		{
			for (int y = 0; y < LARGURA_REDE; ++y)
			{
				if (y != LARGURA_REDE-1)
				{
					network[x][y]->buffer_leste->data(network[x][y+1]->sinal_mux_out_oeste);
				}
				if (y != 0)
				{
					network[x][y]->buffer_oeste->data(network[x][y-1]->sinal_mux_out_leste);
				}
				if (x != ALTURA_REDE-1)
				{
					network[x][y]->buffer_sul->data(network[x+1][y]->sinal_mux_out_norte);
				}
				if (x != 0)
				{
					network[x][y]->buffer_norte->data(network[x-1][y]->sinal_mux_out_sul);
				}

			}
		}

		for (int x = 0; x < ALTURA_REDE; ++x)
		{
			for (int y = 0; y < LARGURA_REDE; ++y)
			{
				if (x == 0)
				{	
					network[x][y]->buffer_norte->data(canais_de_comunicacao_TERRA[contador_canais_TERRA]);
					contador_canais_TERRA++;
				}
				if (y == 0)
				{	
					network[x][y]->buffer_oeste->data(canais_de_comunicacao_TERRA[contador_canais_TERRA]);
					contador_canais_TERRA++;
				}
				if (x == ALTURA_REDE-1)
				{	
					network[x][y]->buffer_sul->data(canais_de_comunicacao_TERRA[contador_canais_TERRA]);
					contador_canais_TERRA++;
				}
				if (y == LARGURA_REDE-1)
				{	
					network[x][y]->buffer_leste->data(canais_de_comunicacao_TERRA[contador_canais_TERRA]);					
					contador_canais_TERRA++;
				}

			}
		}		


	}
};

#endif
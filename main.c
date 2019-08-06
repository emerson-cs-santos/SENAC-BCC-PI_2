#include <allegro.h>
#include <stdio.h> 
#include <stdlib.h> 

#define MAX_RANK    10

// Struct da lista
struct Bolha { int nX, nY, nVal; };

// Altura e largura da tela
int nAltura = 768, nLargura =1024, nRegraMatematica = 0;

struct Bolha VetorBolha[15];

//////////////////////////////////////////////////////////////////////
// Funções utilizadas durante o programa
BITMAP *RetornaPersonagem( float *nIndex, int nDirecao );
BITMAP *RetornaPersonagemParado( int nDirecao );

int ExecuteJogoBolhas( );
int ExecuteCorredorBolhas( );
int ValidRegra( int nVal );
int VerifPosPersonagem( int nPosX, int nPosY, int nDirecao, int nQtVetor );
int ContarDigitos( int nVal );
int ExibirBolha( int nQtBolha, FONT *oFont, BITMAP *bmp );
int IniciarBolha( int nQtBolha, int nBolhasVazias, int xPers, int yPers );
int menu();

void sobre();
void tutorial();
void mudacor( int lnS );
void MensagemSucesso();
volatile int i=0;

void imaismais() { i++; }
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Programa principal
int main()
{ 
    allegro_init();
    install_keyboard();

    install_mouse();
    enable_hardware_cursor();

    install_timer(); 
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
    install_int(imaismais,100);
    LOCK_VARIABLE(i);
    LOCK_FUNCTION(imaismais);

    set_color_depth(32);
    
    set_window_title("MatematicaDinamica"); 
    // Caso nao consiga iniciar, finaliza o jogo
    
    if( set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, nLargura, nAltura,0,0) != 0 )
    {
        exit(0);
    }
    
    int lnRE;
     
    // Menu do jogo       
    do{
        
        lnRE = menu();   
        
        // START
        if ( lnRE == 0 )
        {
           ExecuteCorredorBolhas();
        }
    
        // Tutorial
        if ( lnRE == 1 )
        {
          tutorial();
        }

        // Sobre
        if ( lnRE == 2 )
        {
          sobre();
        }
         
    }while( lnRE < 3 );
    
    return 1;
}
END_OF_MAIN();
////////////////////////////////////////////////////////////////////////////////////////////

void sobre(){
    PALETTE pal;
    BITMAP *menu = load_bitmap("sobre.bmp",pal);
    draw_sprite(screen, menu, 1, 1 ); // exibe imagem  

    SAMPLE *som_t = load_wav("sobre.wav");
    play_sample (som_t, 255, 128, 1000, 10);  

    while(!key[KEY_ESC])
    {

      
	}
    readkey();
    stop_sample(som_t);
}

void tutorial(){
    PALETTE pal;
    BITMAP *tutorial = load_bitmap("tutorial.bmp",pal);
    draw_sprite(screen, tutorial, 1, 1 );
    SAMPLE *som_k = load_wav("tutorial.wav");
    play_sample (som_k, 255, 128, 1000, 10); 
    while(!key[KEY_ESC])
    { 
	}
    readkey();
    stop_sample(som_k);
}

// Menu principale da tela
int menu()
{
    PALETTE pal;
    BITMAP *menu = load_bitmap("menu_principal.bmp",pal);
    draw_sprite(screen, menu, 0, 0 ); // exibe imagem
    FONT *font_menu = load_font ("font2.pcx", NULL, makecol( 35,100,255));
    //textprintf_ex(screen,font_menu,250,200,makecol(0,0,255),-1,"Matematica Dinamica");
    SAMPLE *som_w = load_wav("menu.wav");
    SAMPLE *som_z = load_wav("menuP.wav");
    play_sample (som_z, 255, 128, 1000, 10);  

    int lnSEL = 0;
    int nApertou = 0;
    
    mudacor( lnSEL ); 
    // Equanto nao pressionar a tecla enter, continua no menu principal
    while(!key[KEY_ENTER] )
    {   
       // Botao superior
       if(key[KEY_UP] && nApertou == 0) 
          {
            play_sample (som_w, 255, 128, 1000, 0);          
            nApertou = 1;

            if ( lnSEL == 0 )
            { lnSEL = 4; }
            else
            { lnSEL = lnSEL - 1; }
              
            mudacor( lnSEL );                      
          } 
       
       // Botao inferior
       else if(key[KEY_DOWN] && nApertou == 0)
          {
            play_sample (som_w, 255, 128, 1000, 0);
            nApertou = 1;
            
            if ( lnSEL == 4 )
            { lnSEL = 0; }
            else
            { lnSEL = lnSEL + 1; }
            
            mudacor( lnSEL );                      
            
          } 
          else 
          { nApertou = 0; }
      rest(80);                                                 
    };    
    stop_sample(som_z);
    return lnSEL; 
}

// Muda a cor ao selecionar uma opcao do menu principal
void mudacor( int lnS )
{
       // Destaque o Iniciar
       if ( lnS == 0 )
       {
          textprintf_ex(screen,font,470,500,makecol(255,255,255),-1,"Inicio");
          textprintf_ex(screen,font,470,550,makecol(0,0,0),-1,"Tutorial");
          textprintf_ex(screen,font,470,600,makecol(0,0,0),-1,"Sobre");
          textprintf_ex(screen,font,470,650,makecol(0,0,0),-1,"Fechar");
       }

       // Destaque o Tutorial
       if ( lnS == 1 )
       {
          textprintf_ex(screen,font,470,500,makecol(0,0,0),-1,"Inicio");
          textprintf_ex(screen,font,470,550,makecol(255,255,255),-1,"Tutorial");
          textprintf_ex(screen,font,470,600,makecol(0,0,0),-1,"Sobre");
          textprintf_ex(screen,font,470,650,makecol(0,0,0),-1,"Fechar"); 
       }

       // Destaque o Sobre
       if ( lnS == 2 )
       {
         
          textprintf_ex(screen,font,470,500,makecol(0,0,0),-1,"Inicio");
          textprintf_ex(screen,font,470,550,makecol(0,0,0),-1,"Tutorial");
          textprintf_ex(screen,font,470,600,makecol(255,255,255),-1,"Sobre");
          textprintf_ex(screen,font,470,650,makecol(0,0,0),-1,"Fechar"); 
       }

       // Destaque o Fechar
       if ( lnS == 3 )
       {
          textprintf_ex(screen,font,470,500,makecol(0,0,0),-1,"Inicio");
          textprintf_ex(screen,font,470,550,makecol(0,0,0),-1,"Tutorial");
          textprintf_ex(screen,font,470,600,makecol(0,0,0),-1,"Sobre");
          textprintf_ex(screen,font,470,650,makecol(255,255,255),-1,"Fechar");
       }
       
}

// Retorna o personagem a ser exibido na tela
BITMAP *RetornaPersonagem( float *nIndex, int nDirecao )
{
       BITMAP *pPersonagem = NULL;
       
       // Cima
       if( nDirecao == 2)
       {
         if( *nIndex <= 0 || *nIndex >= 8 ) { pPersonagem = load_bitmap( "Up1.bmp", NULL ); *nIndex = 1; }
         if ( *nIndex > 0 && *nIndex < 4  )   pPersonagem = load_bitmap( "Up2.bmp", NULL );
         if ( *nIndex >= 4 && *nIndex < 8 )   pPersonagem = load_bitmap( "Up1.bmp", NULL );
       }
       
       // Baixo
       if( nDirecao == -2)
       {
         if( *nIndex >= 0 || *nIndex <= -8  ) { pPersonagem = load_bitmap( "Dowm0.bmp", NULL ); *nIndex = -1; }
         if ( *nIndex < 0 && *nIndex > -4   )   pPersonagem = load_bitmap( "Dowm1.bmp", NULL );
         if ( *nIndex <= -4 && *nIndex > -8 )   pPersonagem = load_bitmap( "Dowm2.bmp", NULL );
       }
       
       // Direita
       if( nDirecao == 1)
       {
         if( *nIndex <= 0 || *nIndex >= 8 ) { pPersonagem = load_bitmap( "Right1.bmp", NULL ); *nIndex = 1; }
         if ( *nIndex > 0 && *nIndex < 4  )      pPersonagem = load_bitmap( "Right2.bmp", NULL );
         if ( *nIndex >= 4 && *nIndex < 8 )     pPersonagem = load_bitmap( "Right1.bmp", NULL );
       }
       
       // Esquerdas
       if( nDirecao == -1)
       {
         if( *nIndex >= 0 || *nIndex <= -8  ) { pPersonagem = load_bitmap( "Left1.bmp", NULL ); *nIndex = -1; }
         if ( *nIndex < -0 && *nIndex > -4)     pPersonagem = load_bitmap( "Left2.bmp", NULL );
         if ( *nIndex <= -4 && *nIndex > -8)    pPersonagem = load_bitmap( "Left1.bmp", NULL );
       }
      
       // Index positivo
       if( nDirecao >0 ) *nIndex += 1 ;
       
       // Index negativo
       if( nDirecao <0 ) *nIndex -= 1 ;
       
       rest(50);
       
       return pPersonagem;
}

// Retorna personagem parado
BITMAP *RetornaPersonagemParado( int nDirecao )
{
       rest(50);
       
       BITMAP *pPersonagem = NULL;
       if( nDirecao == -1) pPersonagem = load_bitmap( "Left0.bmp", NULL );
       if( nDirecao == 1 ) pPersonagem = load_bitmap( "Right0.bmp", NULL );
       if( nDirecao == -2) pPersonagem = load_bitmap( "Dowm0.bmp", NULL );
       if( nDirecao == 2 ) pPersonagem = load_bitmap( "Up0.bmp", NULL );
       
       return pPersonagem;
}

// Inicia o jogo de coleta de nrs.
int ExecuteJogoBolhas( )
{
    float nIndex = 0;
    int x = (int) nLargura / 2, y = nAltura / 2, nVelocidade = 10, nDirecao= 0, i = 0,nQtVetor = 20;
    int nQtVazio,nIniciarBolha=0,nReturn;
    
    BITMAP *bmp = create_bitmap(nLargura, nAltura);
    BITMAP *imgErrou = load_bitmap( "Errou.bmp", NULL );
    BITMAP *character = load_bitmap("Dowm0.bmp", NULL);; 
    BITMAP *imgFundo, *imgDesc;
    
    // Som do jogo
	SAMPLE *som_a = load_wav("tela1.wav");
	SAMPLE *som_b = load_wav("tela3.wav");
	SAMPLE *som_c = load_wav("tela2.wav");
    
    // A imagem a ser carregada, depende da regra
    if( nRegraMatematica == 0 ) 
    { 
        imgFundo = load_bitmap("floor.bmp", NULL); 
        imgDesc  = load_bitmap("primos.bmp", NULL);
        play_sample (som_a, 255, 128, 1000, 10);      
    }else if( nRegraMatematica == 1 ) 
    { 
        play_sample (som_b, 255, 128, 1000, 10);  
        imgFundo = load_bitmap("deserto.bmp", NULL); 
        imgDesc  = load_bitmap("pares.bmp", NULL);
    }else 
    { 
        play_sample (som_c, 255, 128, 1000, 10);  
        imgFundo = load_bitmap("grama.bmp", NULL); 
        imgDesc  = load_bitmap("impares.bmp", NULL);
    }

    // Zera o valor do vetor
    for( i = 0; i < nQtVetor ; i ++ ) 
    {
         VetorBolha[i].nVal = -1; 
    }
    
    // Exibe a imagem em branco no console
    blit(bmp, screen, 0, 0, 0,0 ,(*bmp).w, (*bmp).h);
    // Remove referencia da imagem
    clear(bmp);
    // Printa a imagem de fundo na tela
    draw_sprite( bmp, imgFundo, 0, 0 );
    
    // Printa o personagem na tela 
    draw_sprite(bmp, character, x, y);
    draw_sprite(screen, imgDesc, 80, 300); 
    
    // Fonte customizada
    FONT *oFont = load_font( "fonte1.pcx", NULL, NULL );
    
    // Segura 2 segundos para iniciar o jogo
    rest( 2000 );
    
    // Carrega os numeros durante o jogo
    IniciarBolha( nQtVetor, nQtVetor, x, y );
     
    int nQtRodar = 2;
    
    // Enquanto nao pressionar a tecla enter, ou nao completar o jogo continua com o jogo aberto
    while(!key[KEY_ESC] && nIniciarBolha < nQtRodar)
    {
     
        // Botão esquerdo 
        if (key [KEY_LEFT]) 
        {
            nDirecao = -1;
            character = RetornaPersonagem( &nIndex, nDirecao );
            
            if( character != NULL )
            { 
                x-= nVelocidade;
                if( x <= 0 ) x = nLargura-10;
            }
            
        }
        
        // Botão direito
        else if (key [KEY_RIGHT]) 
        {
            nDirecao = 1;
            character = RetornaPersonagem( &nIndex, nDirecao );
            
            if( character != NULL )
            { 
                x+= nVelocidade;
                if( x > nLargura-30 ) x = -15;
            }
            
        }
        // Botão superior 
        else if (key [KEY_UP]) 
        {
           nDirecao = 2;
           character = RetornaPersonagem( &nIndex, nDirecao );
           
           if( character != NULL ) 
           {
               y-= nVelocidade;
               if( y <= -35 ) y = nAltura-2; 
           }
           
        }
        
        // Botão inferior 
        else if (key [KEY_DOWN]) 
        {
           nDirecao = -2;
           character = RetornaPersonagem( &nIndex, nDirecao );
           
           if( character != NULL ) 
           {
               y+= nVelocidade;
               if( y>= nAltura ) y = -30;
           }
           
        }
        // Retorna o personagem parado 
        else
        {
              character = RetornaPersonagemParado( nDirecao );
        }
                
        // Limpa a tela para atualizar as informações
        blit(bmp, screen, 0, 0, 0,0 ,(*bmp).w, (*bmp).h);
        
        // Valores a Serem Exibidos
        nQtVazio = ExibirBolha( nQtVetor, oFont, bmp ); 
        
        // Caso não encontre o personagem, não faz nada
        if( character != NULL )
        {
            // Remove referencia da imagem
            clear(bmp);
            // Printa a imagem de fundo na tela
            draw_sprite( bmp, imgFundo, 0, 0 );
            
            // Printa o personagem na tela 
            draw_sprite(bmp, character, x, y);
            nReturn = VerifPosPersonagem( x, y, nDirecao, nQtVetor );
            
            // verifisca se pegou o valor errado
            if ( nReturn == -1 ) 
            { 

                if( nRegraMatematica == 0 ) 
                { 
                  stop_sample(som_a);    
                }
                else if( nRegraMatematica == 1 ) 
                { 
                  stop_sample(som_b);
                }
                else 
                { 
                  stop_sample(som_c);
                }

                SAMPLE *som_l = load_wav("erro.wav");
                play_sample (som_l, 255, 128, 1000, 20);
              
                while( ! key[KEY_ESC] ) 
                {
                     rest(50);
                     draw_sprite( screen, imgErrou, 200, 300 );
                }
                stop_sample(som_l);

                return -1; 
            }
         } 

        // Valores a Serem Exibidos
        nIniciarBolha += IniciarBolha( nQtVetor, nQtVazio, x, y ); 
 
    }
    
    // A imagem a ser carregada, depende da regra
    if( nRegraMatematica == 0 ) 
    { 
      stop_sample(som_a);    
    }
    else if( nRegraMatematica == 1 ) 
    { 
      stop_sample(som_b);
    }
    else 
    { 
      stop_sample(som_c);
    }
    
    // Remove as imagens da memoria
    destroy_bitmap(bmp);
    destroy_bitmap(character);
    destroy_bitmap(imgFundo);
    
    // Verifica se foi completado a rodada, e retorna 1
    if( nIniciarBolha < nQtRodar )
        { return 0; } 
        else
        { return 1; }
}

// Executa o corredor do jogo, escolha de fases
int ExecuteCorredorBolhas( )
{
    float nIndex = 0;
    int x = (int) nLargura / 2, y = nAltura / 2, nVelocidade = 10, nDirecao= 0, i = 0,nQtVetor = 20;
    int nQtVazio,nVal_TelaCim = 0,nVal_TelaEsq = 0, nVal_TelaDir = 0, nReturn,nDifTempo = 0 ;

    struct myTime{ time_t Init, Fim;  };
    struct myTime myTempo;
    time( &myTempo.Init ); 
    
    BITMAP *bmp = create_bitmap(nLargura, nAltura);
    BITMAP *imgFundo = load_bitmap("corredor.bmp", NULL);
    BITMAP *character = load_bitmap("Dowm0.bmp", NULL);; 

    SAMPLE *som_y = load_wav("corredor.wav");
    play_sample (som_y, 255, 128, 1000, 10);  
    
    // Zera o valor do vetor
    for( i = 0; i < nQtVetor ; i ++ ) 
    {
         VetorBolha[i].nVal = -1; 
    }
    
    // Limpa a tela para atualizar as informações
    blit(bmp, screen, 0, 0, 0,0 ,(*bmp).w, (*bmp).h);
    // Remove referencia da imagem
    clear(bmp);
    
    // Printa a imagem de fundo na tela
    draw_sprite( bmp, imgFundo, 0, 0 );

    // Printa o personagem na tela 
    draw_sprite(bmp, character, x, y); 
    
    // Font dos Numeros
    FONT *oFont = load_font( "fonte1.pcx", NULL, NULL );
    
    int pos,x_pos,y_pos;

    // Pontuação
    textprintf_ex(screen,oFont,800,0,makecol(0,0,0),-1, "Gramado: %i", nVal_TelaEsq );
    textprintf_ex(screen,oFont,800,40,makecol(0,0,0),-1,"Casa:   %i", nVal_TelaCim );
    textprintf_ex(screen,oFont,800,80,makecol(0,0,0),-1,"Deserto: %i", nVal_TelaDir);

    while( ! ( x>=492 && x<=542 && y >= 714 ) )
    {
        // Botão esquerdo 
        if (key [KEY_LEFT]) 
        {
            nDirecao = -1;
            character = RetornaPersonagem( &nIndex, nDirecao );
            
            if( character != NULL )
            { 
                x-= nVelocidade;
                
                // Grama ( fase da esquerda )
                if( x <= 31 && y + 40 >= 392 && y <= 534 ) 
                {
                    // Espaco par entrar na "porta"
                    if( y>= 424 && y<= 484 )
                    {
                        nRegraMatematica = 2;
                        stop_sample(som_y);
                        time( &myTempo.Init );
                        nReturn = ExecuteJogoBolhas();
                        
                        // Caso retorne valor maior que 0, eh pq foi completado
                        if( nReturn > 0 )
                        {
                            nVal_TelaEsq =+ nReturn;
                            time( &myTempo.Fim ); 
                            nDifTempo += ( myTempo.Fim - myTempo.Init );
                            MensagemSucesso( );
                        }
                        play_sample (som_y, 255, 128, 1000, 10);  
                        x = 35;
                    }
                    else
                    {
                        x += nVelocidade;
                    }
                }
                
                // Casa superior
               if( y <= 223 && x >= 222 && x <= 785 ) x += nVelocidade;
               
               if( x <= 0 ) x += nVelocidade;
            }
        }
        
        // Botão direito
        else if (key [KEY_RIGHT]) 
        {
            nDirecao = 1;
            character = RetornaPersonagem( &nIndex, nDirecao );
            
            if( character != NULL )
            { 
                x+= nVelocidade;
                // Cerca da direita
                if( x >=972 && y >= 344 && y <= 564 ) 
                {
                   if( y >= 424 &&  y<=504 )
                   {

                        nRegraMatematica = 1;
                        stop_sample(som_y);
                        time( &myTempo.Init );
                        nReturn = ExecuteJogoBolhas();
                        if( nReturn > 0 )
                        {
                            nVal_TelaDir += nReturn;
                            time( &myTempo.Fim ); 
                            nDifTempo += ( myTempo.Fim - myTempo.Init ); 
                            MensagemSucesso();
                        }
                        play_sample (som_y, 255, 128, 1000, 10);
                        x = 971 ;
                    
                    } else {
                        x -= nVelocidade;
                    }
                }
                
                // Casa superior
                if( y <= 224 && x >= 229 && x <= 785 ) x -= nVelocidade;
                
                // Total da tela
                if( x > nLargura-30 ) x -= nVelocidade;
                
            }
            
        }
        // Botão superior 
        else if (key [KEY_UP]) 
        {
           nDirecao = 2;
           character = RetornaPersonagem( &nIndex, nDirecao );
           
           if( character != NULL ) 
           {
               y-= nVelocidade;

               // Cerca da esquerda
               if( x <= 31 && y + 40 >= 324 && y <= 544 ) y += nVelocidade;
               
               // Cerca da direita
               if( x >=972 && y >= 354 && y <= 564 ) y += nVelocidade;
               
               // Casa superior
               if( y <= 224 && x >= 229 && x <= 785 )
               {
                   if( x>=492 && x<=522 )
                   {
                       nRegraMatematica = 0;
                       stop_sample(som_y);
                       time( &myTempo.Init );
                       nReturn = ExecuteJogoBolhas();
                       if( nReturn > 0 )
                       {
                           nVal_TelaCim += nReturn;
                           time( &myTempo.Fim ); 
                           nDifTempo += ( myTempo.Fim - myTempo.Init ); 
                           MensagemSucesso();
                       }
                       play_sample (som_y, 255, 128, 1000, 10);
                       y = 225 ;
                   }
                   else
                   {
                         y += nVelocidade;
                   } 
               }

               // Total da tela
               if( y <= -10 ) y += nVelocidade; 
           }
           
        }
        
        // Botão inferior 
        else if (key [KEY_DOWN]) 
        {
           nDirecao = -2;
           character = RetornaPersonagem( &nIndex, nDirecao );
           
           if( character != NULL ) 
           {
               y+= nVelocidade;
               
               // Cerca da esquerda
               if( x <= 31 && y + 40 >= 384 && y <= 534 ) y -= nVelocidade;
               
               // Cerca da direita
               if( x >=972 && y >= 344 && y <= 564 ) y -= nVelocidade;
               
               // Total da tela
               if( y >= nAltura - 50 ) y -= nVelocidade;
               
           }
           
        }
        // Retorna o personagem parado 
        else
        {
              character = RetornaPersonagemParado( nDirecao );
        }
                
        // Limpa a tela para atualizar as informações
        blit(bmp, screen, 0, 0, 0,0 ,(*bmp).w, (*bmp).h);
        
        // Quantida de vezes as telas foram passadas
        if( nVal_TelaCim < 0 ) nVal_TelaCim = 0;
        if( nVal_TelaDir < 0 ) nVal_TelaDir = 0;
        if( nVal_TelaEsq < 0 ) nVal_TelaEsq = 0;
        
        // Caso não encontre o personagem, não faz nada
        if( character != NULL )
        {
            // Remove referencia da imagem
            clear(bmp);
            // Printa a imagem de fundo na tela
            draw_sprite( bmp, imgFundo, 0, 0 );
            
            // Printa o personagem na tela 
            draw_sprite(bmp, character, x, y);
            
         } 
        // Pontuação
        textprintf_ex(screen,oFont,800,0,makecol(0,0,0),-1, "Gramado: %i", nVal_TelaEsq );
        textprintf_ex(screen,oFont,800,40,makecol(0,0,0),-1,"Casa:   %i", nVal_TelaCim );
        textprintf_ex(screen,oFont,800,80,makecol(0,0,0),-1,"Deserto: %i", nVal_TelaDir);
    
    }
     
     // Remove as variaveis da memoria
     stop_sample(som_y);
     destroy_bitmap(bmp);
     destroy_bitmap(character);
     destroy_bitmap(imgFundo);
          
 return 0;
}

// Controle os nr`s aleatorios do jogo
int IniciarBolha( int nQtBolha, int nBolhasVazias, int xPers, int yPers )
{
     
     int i = 0, j = 0, h = 0;
     int nValMax = 100;
     
     // Valor maximo para os numeros primos
     if( nRegraMatematica == 0 ) nValMax = 50;

     // Varre o vetor para verificar se ainda existe algum valor a ser pego
     for( i = 0; i < nQtBolha ; i ++ )
     {
        if( VetorBolha[i].nVal != -1 && ValidRegra( VetorBolha[i].nVal ) == 1 )
        {
            return 0;
        }  
     }
     
     // Aguarda 150 milesimos para iniciar o jogo
     rest( 150 );
     
     // 
     for( i = 0; i < nQtBolha ; i ++ )
     {
        j = 0;
        VetorBolha[i].nVal = rand() % nValMax;
        
        while( VetorBolha[i].nVal == 0 ) VetorBolha[i].nVal = rand() % nValMax;
        
        while( j <= i )
        {
           if( j == 0 ) 
           {
                VetorBolha[i].nX   = rand() % ( nLargura - 200 );
                VetorBolha[i].nY   = rand() % ( nAltura - 100 );
           }
           if( 
               VetorBolha[j].nX != 0 &&
               VetorBolha[j].nY != 0 &&
               abs( VetorBolha[i].nX  - VetorBolha[j].nX ) < 60 &&
               abs( VetorBolha[i].nY  - VetorBolha[j].nY ) < 70 &&
               j != i
             ) 
             
               { j = 0; }
               else 
               { 
                    if( VetorBolha[j].nX >= xPers && VetorBolha[j].nX <= xPers + 80 && 
                    VetorBolha[j].nY >= yPers && VetorBolha[j].nY <= yPers + 70 )
                    { j = 0; }
                    else
                    { j +=1; }
               }
        }
        
        if( i <= 10 )
        {    
            while( ValidRegra( VetorBolha[i].nVal ) == 0 || VetorBolha[i].nVal == 0 ) 
            { 
                VetorBolha[i].nVal = rand() % nValMax;
            }
        }
     }
     
     return 1;     
}

// Exibe os valores do vetor na tela
int ExibirBolha( int nQtBolha, FONT *oFont, BITMAP *bmp )
{
     int i = 0, nQtVazio = 0;
     for( i = 0; i < nQtBolha ; i ++ )
     {
          if( VetorBolha[i].nVal == -1 )
          {
              nQtVazio += 1;
              
          }else{
            textprintf_ex( screen, oFont,VetorBolha[i].nX, VetorBolha[i].nY, makecol(120,110,200),-1, "%i", VetorBolha[i].nVal );  
          }
     }     
     
     return nQtVazio;
}

// Verifica a posicao do personagem
int VerifPosPersonagem( int nPosX, int nPosY, int nDirecao, int nQtVetor )
{
    int i, nPosAdd;
    
    for( i = 0; i < nQtVetor; i ++ )
    {
        nPosAdd = ContarDigitos( VetorBolha[i].nVal );
        if( nPosX >= VetorBolha[i].nX - 25 && nPosX <= VetorBolha[i].nX + nPosAdd )
        {
            if( nPosY >= VetorBolha[i].nY -40 && nPosY <= VetorBolha[i].nY + 27 )
            {
                if( VetorBolha[i].nVal == -1 ) return 0;
                
                if( ValidRegra( VetorBolha[i].nVal ) == 1 ) 
                {
                    VetorBolha[i].nVal = -1;
                    return 1;
                }else{ return -1; }
            }
        }
    }
    
    return 0;
}

// Calcula quantidade de digitos de uma variavel
int ContarDigitos( int nVal )
{
    int nDigitos = 0;

    nVal = abs( nVal );
    
    do{      
        nVal = (int) nVal / 10;
        nDigitos ++;
        
    }while( nVal > 0 );
    
    switch ( nDigitos )
    {
        case 1:
            nVal = 15;
            break;
        case 2:
            nVal = 35;
            break;
            
        case 3:
            nVal = 45;
            break;
    }

    return nVal;
    
}

// Calculo de nrs primos
int ValidarNrPrimo( int nVal )
{
    int nDiv=1, i=2;
    
    if( nVal == 0 || nVal == 1 ) return 0 ;
    
    while( i < nVal && nDiv != 0 )
    {
      
      nDiv = nVal % i;
      i++;
              
    }
    
    if( nDiv > 0 ) return 1;
    
    return nDiv;
    
}

// Valida a regra dependendo do jogo
int ValidRegra( int nVal )
{
    
    if( nRegraMatematica == 0 )
    {
        return ValidarNrPrimo( nVal );
    } 
    else if ( nRegraMatematica == 1 )
    {
          if( nVal % 2 != 0 ) return 0;
    }
    else
    
    { if( nVal % 2 == 0 ) return 0; }
           
    return 1;
    
}

// Mensagem de suscesso do jogo
void MensagemSucesso()
{
     rest(100);
}

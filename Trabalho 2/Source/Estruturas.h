#define NUM_PAGINAS_PROCESSO 50
#define NUM_FRAMES 64
#define INTERVALO 3 // paginas e processos sendo criados a cada INTERVALO
#define NUM_PROCESSOS 20

typedef struct _Processo {
    int PID;
    int numeroPaginasMemoria;
    int tabelaPaginas[NUM_PAGINAS_PROCESSO];
} Processo;

typedef struct _Pagina {
    int paginaID;
    int PID;
} Pagina;

typedef struct _MemoriaPrincipal {
    Pagina* frames[NUM_FRAMES];
} MemoriaPrincipal;

Processo* CriaProcesso(int PID);
Pagina* CriaPagina(int paginaID, int PID);
MemoriaPrincipal* CriaMemoriaPrincipal();
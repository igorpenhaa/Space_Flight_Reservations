#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define TAM_NUM_VOO 9
#define TAM_NUM_RESERVA 10
#define TAM_CPF 15
#define TAM_NOME 100

// sintese
// objetivo: Fazer um programa que ira cuidar das reservas de viagens espaciais;
// entrada : Podera fazer o cadastro do voo ou  cadastrar reserva ou sair do programa;
// saida   : O usuario podera ou consultar o voo ou consultar a reserva ou consultar o usuario cadastrado ou sair do programa;

struct voo // struct voo tera as informacoes que um voo devera apresentar;
{
    char num_voo[TAM_NUM_VOO];
    int quant_max;
    int dia_voo;
    int mes_voo;
    int ano_voo;
    int hora_partida;
    int min_partida;
    char status_voo;
}; 

struct reserva// struct reserva tera as informacoes que uma reserva devera apresentar;
{
    char num_voo[TAM_NUM_VOO];
    char num_reserva[TAM_NUM_RESERVA];
    char cpf[TAM_CPF];
    char nome_completo[TAM_NOME];
    int sexo;
    int dia_nascimento;
    int mes_nascimento;
    int ano_nascimento;
    char status_reserva;
}; 

void clear() // Limpa o Terminal
{
#if defined(linux) || defined(unix) || defined(APPLE)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

void menu() // A apresntacao do menu de escolha ao usuario;
{
    // o menu que aparecera na tela;

    printf("1) Cadastrar voo        \n");
    printf("2) Cadastrar reserva    \n");
    printf("3) Consultar voo        \n");
    printf("4) Consultar reserva    \n");
    printf("5) Consultar passageiro \n");
    printf("6) Cancelar voo         \n");
    printf("7) Cancelar reserva     \n");
    printf("8) Excluir voo          \n");
    printf("9) Sair do programa     \n\n");
}

void flush_in() // funcao equivalente ao fflush ou getchar, nao estava funcionando no online gbd, entao foi criada uma funcao;
{

    int ch;

    while ((ch = fgetc(stdin)) != EOF && ch != '\n')
    {
    }
}

int valida_data_voo(int dia_voo, int mes_voo, int ano_voo) // funcao em que vai conferir se a data do voo sera uma data valida;
{
    // DECLACOES:
    int valida;
    time_t mytime;
    int dia, mes, ano;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    // INSTRUCOES:
    valida = 0;
    dia = tm.tm_mday;
    mes = tm.tm_mon + 1;
    ano = tm.tm_year + 1900;

    // validando a data;
    // para meses com 31 dias;

    if (ano_voo >= ano) // o ano tem que ser no ano em que se está ou depois
    {
        if (mes_voo >= 1 && mes_voo <= 12) // o mes digitado tem que estar entre 1 e 12, considerando os meses do ano;
        {
            if (mes_voo == 1 || mes_voo == 3 || mes_voo == 5 || mes_voo == 7 || mes_voo == 8 || mes_voo == 10 || mes_voo == 12) //meses com 31 dias;
            { // validando os mes com 31 dias;
                if (dia_voo >= 1 && dia_voo <= 31) // o dia desses meses tem que estar entre 1 e 31;
                {
                    valida = 1;
                }
            }
            else if (mes_voo == 4 || mes_voo == 6 || mes_voo == 9 || mes_voo == 11) // meses com 30 dias;
            { // para os meses com 30 dias;
                if (dia_voo >= 1 && dia_voo <= 30) // o dia desses meses tem que estar entre 1 e 30;
                {
                    valida = 1;
                }
            }
        }
    }
    // para o mês de fevereiro caso seja bissesto;
    if (ano_voo >= 2021)
    {
        if ((ano_voo % 4 == 0 && ano_voo % 100 != 0) || ano_voo % 400 == 0) // para o ano ser bissesto tem que ser divisivel por 4 e 100 ou divisivel por 400;
        {
            if (mes_voo == 2)
            {
                if (dia_voo >= 1 && dia_voo <= 29) // no ano bissesto o mes de fevereiro tem 29 dias;
                {
                    valida = 1;
                }
            }
        }
        else if (ano_voo % 4 != 0)
        { // para fevereiro quando nao e bissesto;
            if (mes_voo == 2)
            {
                if (dia_voo >= 1 && dia_voo <= 28) // no ano nao bissesto o mes de fevereiro tem 28 dias;
                {
                    valida = 1;
                }
            }
        }
    }

    if (dia_voo <= dia + 1 && mes_voo <= mes && ano_voo <= ano) // conferindo com a data do computador;
    {
        valida = 0;
    }

    // se estiver ok, validar sera igual a 1 se houver algum erro validar sera igual a 0
    return valida;
}

int valida_hora_voo(int hora_partida, int min_partida) // funcao em que vai validar a hora de partida do voo;
{
    // DECLARACOES:
    int valida;

    // INSTRUCOES:
    valida = 0;

    // saber se a hora de partida do voo e valida;
    if (hora_partida >= 0 && hora_partida <= 23)
    {
        if (min_partida >= 0 && min_partida <= 59)
        {
            valida = 1;
        }
    }

    return valida;
}

/**
 * Retorna :
 *  1 se for valido
 *  0 se for invalido
 * -1 se for repetido
 *
 */
int valida_numero_voo(char numero_voo[], FILE *arquivo1)
{
    // DECLACOES:
    char conteudo_numero_voo[TAM_NUM_VOO];
    struct voo voo_busca;

    // INSTRUCOES:

    // conferindo se a estrutura do numero de voo esta correta, tem que seguir a estrutura JEB-XXXX, em que X sao numeros naturais;
    if (!(strlen(numero_voo) == 8 && numero_voo[0] == 'J' && numero_voo[1] == 'E' && numero_voo[2] == 'B' && numero_voo[3] == '-' && isdigit(numero_voo[4]) && isdigit(numero_voo[5]) && isdigit(numero_voo[6]) && isdigit(numero_voo[7])))
    {
        return 0;
    }

    fseek(arquivo1, 0, SEEK_SET);
    // conferindo se o numero do voo cadastrante ja nao foi previamente cadastrado;
    while (fread(&voo_busca, sizeof(struct voo), 1, arquivo1))
    {
        if (strcmp(voo_busca.num_voo, numero_voo) == 0) // se dentro do arquivo ja houver um cadastro com esse numero, ele deve ser invalido por ja ter sido utilizado;
        {
            return -1;
        }
    }
    return 1;
}

int valida_numero_reserva(char numero_reserva[], FILE *arquivo2)
{
    // DECLACOES:
    int valida;
    struct reserva reserva_busca;

    // INSTRUCOES:

    // conferindo se a estrutura do numero de reserva esta correta;
    if (!(strlen(numero_reserva) == 9 && numero_reserva[0] == 'G' && numero_reserva[1] == 'B' && numero_reserva[2] == '-' && isdigit(numero_reserva[3]) && isdigit(numero_reserva[4]) && isdigit(numero_reserva[5]) && isdigit(numero_reserva[6]) && isdigit(numero_reserva[7]) && isdigit(numero_reserva[8])))
    {
        return 0;
    }

    fseek(arquivo2, 0, SEEK_SET);
    // conferindo se aquele numero de reserva ja nao foi utilizado antes;
    while (fread(&reserva_busca, sizeof(struct voo), 1, arquivo2))
    {
        if (strcmp(reserva_busca.num_reserva, numero_reserva) == 0) // se dentro do arquivo ja houver um cadastro com esse numero, ele deve ser invalido por ja ter sido utilizado;
        {
            return 2;
        }
    }

    return 1;
}

int valida_num_voo_reserva(char numero_voo[], FILE *arquivo1)
{
    // DECLACOES:
    char conteudo_numero_voo[11];
    struct voo voo_busca;

    // INSTRUCOES:
    fseek(arquivo1, 0, SEEK_SET);
    // saber se o voo daquela reserva realmente existe;
    // para fazer o cadastro de uma reserva ele tem que ser feito em um voo ja existente;
    while (fread(&voo_busca, sizeof(struct voo), 1, arquivo1))
    {
        if (strcmp(voo_busca.num_voo, numero_voo) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/**
 * Retorna 1 se for valido
 * Retorna 0 se for invalido
 */
int valida_CPF(char CPF_passageiro[], char numero_voo[], FILE *arquivo1, FILE *arquivo2)
{
    // DECLACOES:
    int valida, soma, valor, valores, result, resultado, multiplicacao, mult, cpf[TAM_CPF], i;
    struct voo voo_busca;
    struct reserva cpf_busca;

    // INSTRUCOES:
    valida = 0;
    multiplicacao = 0;
    resultado = 0;
    

    // conferindo se a estrutura do cpf e valida;
    // o cpf tem que seguir a estrutura xxx.xxx.xxx-xx, em que x sao digitos numericos;
    if (!(isdigit(CPF_passageiro[0]) && isdigit(CPF_passageiro[1]) && isdigit(CPF_passageiro[2]) && CPF_passageiro[3] == '.' && isdigit(CPF_passageiro[4]) && isdigit(CPF_passageiro[5]) && isdigit(CPF_passageiro[6]) && CPF_passageiro[7] == '.' && isdigit(CPF_passageiro[8]) && isdigit(CPF_passageiro[9]) && isdigit(CPF_passageiro[10]) && CPF_passageiro[11] == '-' && isdigit(CPF_passageiro[12]) && isdigit(CPF_passageiro[13])))
    {
        return 0;
    } 
    else if ((strcmp(CPF_passageiro, "000.000.000-00") == 0) || (strcmp(CPF_passageiro, "111.111.111-11") == 0) || (strcmp(CPF_passageiro, "222.222.222-22") == 0) || (strcmp(CPF_passageiro, "333.333.333-33") == 0) || (strcmp(CPF_passageiro, "444.444.444-44") == 0) || (strcmp(CPF_passageiro, "555.555.555-55") == 0) || (strcmp(CPF_passageiro, "666.666.666-66") == 0) || (strcmp(CPF_passageiro, "777.777.777-77") == 0) || (strcmp(CPF_passageiro, "888.888.888-88") == 0) || (strcmp(CPF_passageiro, "999.999.999-99") == 0))
    { // numeros de cpf com todos os digitos iguais nao sao validos;
        return 0;
    }
    else
    {
        for(i=0; i<14;i++){ // transformando o caracter numero da string que rstava com o o seu valor da tabela ASCII em numeros inteiros;
            if(i != 3 && i != 7 && i != 11){
                cpf[i] = CPF_passageiro[i] - '0';
            }
        }
        
        valor = (cpf[0] * 10) + (cpf[1] * 9) + (cpf[2] * 8) + (cpf[4] * 7) + (cpf[5] * 6) + (cpf[6]) * 5 + (cpf[8] * 4) + (cpf[9] * 3) + (cpf[10] * 2);
        // Primeiramente multiplica-se os 9 primeiros dígitos pela sequência decrescente de números de 10 à 2 e soma os resultados;
        mult = valor * 10;
        // multiplicarmos esse resultado por 10 e dividirmos por 11;
        result = mult % 11;
        //O resultado que nos interessa na verdade é o RESTO da divisão. Se ele for igual ao primeiro dígito verificador (primeiro dígito depois do '-'), a primeira parte da validação está correta;
        // se o resto for 10, é para considerar 0; 
        if (result == 10)
        {
            result = 0;
        }
        
        
        if (result == cpf[12])
        {
            //A validação do segundo dígito é semelhante à primeira, porém vamos considerar os 9 primeiros dígitos, mais o primeiro dígito verificador, e vamos multiplicar esses 10 números pela sequencia decrescente de 11 a 2;
            
            valores = (cpf[0] * 11) + (cpf[1] * 10) + (cpf[2] * 9) + (cpf[4] * 8) + (cpf[5] * 7) + (cpf[6] * 6) + (cpf[8] * 5) + (cpf[9] * 4) + (cpf[10] * 3) + (cpf[12] * 2);
            
            //Seguindo o mesmo processo da primeira verificação, multiplicamos por 10 e dividimos por 11;
            
            multiplicacao = valores * 10;
            
            resultado = multiplicacao % 11;
            
            if (resultado == 10)
            {
                resultado = 0;
            }
            
            //Verificamos, se o resto corresponde ao segundo dígito verificador;
            
            if (resultado == cpf[13])
            {
                // se todas as opcoes forem atendidas o cpf é valido
                valida = 1;
            }
        }
    }

    fseek(arquivo1, 0, SEEK_SET);
    // conferindo se ja nao ha um cadastro com um mesmo cpf no voo;
    while (fread(&voo_busca, sizeof(struct voo), 1, arquivo1))
    {
        if (strcmp(voo_busca.num_voo, numero_voo) == 0)
        { // Comparacao para confirmar se o cpf digitado esta no arquivo;
            while (fread(&cpf_busca, sizeof(struct reserva), 1, arquivo2))
            {
                if (strcmp(cpf_busca.cpf, CPF_passageiro) == 0) // caso haja um cadastro do mesmo cpf no voo ele deve ser invalido;
                {
                    return 0;
                }
            }
        }
    }

    return valida;
}

int valida_nome(char nome_completo[])
{
    // DECLACOES
    int valida, tamanho;
    int i;

    // INSTRUCOES
    valida = 0;
    for (i = 0; i < strlen(nome_completo); i++) // o nome deve ser composto apenas por letras maiusculas e minusculas e por espaco, qualquer digitacao diferente disso fara com que o nome seja invalido;
    {
        if ((nome_completo[i] > 64 && nome_completo[i] < 91) || (nome_completo[i] > 96 && nome_completo[i] < 123) || nome_completo[i] == 32)
        {
            valida++;
        }
    }

    tamanho = strlen(nome_completo);
    // o tamanho do nome sera validado com a utilizacao de um contador e da funcao strlen;
    // strlen ira pegar o tamanho do nome com todos os caracteres utilizados sejam validos ou nao;
    // o contador so conta se os caracteres forem validos;
    
    if (valida == tamanho) // se tiverem o mesmo tamanho, o nome e valido
    {
        valida = 1;
    }
    else // caso contrario, invalido;
    {
        valida = 0;
    }

    return valida;
}

int valida_nascimento(int dia_nascimento, int mes_nascimento, int ano_nascimento)
{
    // DECLACOES
    int valida;

    // INSTRUCOES
    valida = 0;

    if (ano_nascimento >= 1900) // o homem mais velho do mundo tem 120 anos, entao qualquer cadastro de alguem com mais idade sera considerado invalido;
    {
        if (mes_nascimento >= 1 && mes_nascimento <= 12) // o mes digitado deve ser considerado entre 0 e 12, considerando que sao meses do ano;
        {
            if (mes_nascimento == 1 || mes_nascimento == 3 || mes_nascimento == 5 || mes_nascimento == 7 || mes_nascimento == 8 || mes_nascimento == 10 || mes_nascimento == 12)
            { // para os meses com 31 dias;
                if (dia_nascimento >= 1 && dia_nascimento <= 31)
                {
                    valida = 1;
                }
            }
            else if (mes_nascimento == 4 || mes_nascimento == 6 || mes_nascimento == 9 || mes_nascimento == 11)
            { // para os meses com 30 dias;
                if (dia_nascimento >= 1 && dia_nascimento <= 30)
                {
                    valida = 1;
                }
            }
        }
    }
    // para o mês de fevereiro caso seja bissesto;
    if (ano_nascimento >= 1900)
    {
        if ((ano_nascimento % 4 == 0 && ano_nascimento % 100 != 0) || ano_nascimento % 400 == 0) // em ano bissesto o ano e divisivel por 4 e 100 ou simplesmente por 400;
        {
            if (mes_nascimento == 2)
            {
                if (dia_nascimento >= 1 && dia_nascimento <= 29) // em ano bissesto o mes de fevereiro tem 29 dias;
                {
                    valida = 1;
                }
            }
        }
        else if (ano_nascimento % 4 != 0)
        { // para fevereiro quando nao e bissesto;
            if (mes_nascimento == 2)
            {
                if (dia_nascimento >= 1 && dia_nascimento <= 28) // em anos nao bissesto o mes de fevereiro tem 28 dias;
                {
                    valida = 1;
                }
            }
        }
    }

    return valida;
}

int valida_cancelar_voo(char numero_voo[], FILE *voar)
{
    // DECLACOES:
    char conteudo_numero_voo[TAM_NUM_VOO];
    struct voo voo_busca;

    // INSTRUCOES:

    // conferindo se a estrutura do numero de voo esta correta;
    // a estrutura deve ser JEB-XXXX, com x sendo digitos numericos;
    if (!(strlen(numero_voo) == 8 && numero_voo[0] == 'J' && numero_voo[1] == 'E' && numero_voo[2] == 'B' && numero_voo[3] == '-' && isdigit(numero_voo[4]) && isdigit(numero_voo[5]) && isdigit(numero_voo[6]) && isdigit(numero_voo[7])))
    {
        return 0;
    }

    fseek(voar, 0, SEEK_SET);
    // conferindo se o numero do voo realmente existe ou se ainda nao foi cadastrado;
    while (fread(&voo_busca, sizeof(struct voo), 1, voar))
    {
        if (strcmp(voo_busca.num_voo, numero_voo) == 0)
        {
            return 1;
        }
    }
    return -1;
}

int valida_cancelar_reserva(char numero_reserva[], FILE *reservar)
{
    // DECLACOES:
    int valida;
    struct reserva reserva_busca;

    // INSTRUCOES:

    // conferindo se a estrutura do numero de reserva esta correta;
    // a estrutura de ser GB-XXXXXX, com x sendo digitos numericos;
    if (!(strlen(numero_reserva) == 9 && numero_reserva[0] == 'G' && numero_reserva[1] == 'B' && numero_reserva[2] == '-' && isdigit(numero_reserva[3]) && isdigit(numero_reserva[4]) && isdigit(numero_reserva[5]) && isdigit(numero_reserva[6]) && isdigit(numero_reserva[7]) && isdigit(numero_reserva[8])))
    {
        return 0;
    }

    fseek(reservar, 0, SEEK_SET);
    // conferindo se aquele numero de reserva realmente existe ou se nao foi cadastrado anteriormente;
    while (fread(&reserva_busca, sizeof(struct voo), 1, reservar))
    {
        if (strcmp(reserva_busca.num_reserva, numero_reserva) == 0)
        {
            return 1;
        }
    }

    return -1;
}

void cadastrar_voo()
{
    // DECLACOES:
    int valida;
    valida = 1;
    struct voo Voos;
    FILE *arquivo1;

    // INSTRUCOES:

    // abrir arquivo;

    // Se nao tiver um arquivo, ele vai criar automatico
    arquivo1 = fopen("voos.bin", "a+b");

    // manipulando arquivo;
    clear();
    do
    {
        printf("Digite o numero do voo (JEB-XXXX): \n");
        scanf(" %s", Voos.num_voo);
        // verificando numeero voo;
        valida = valida_numero_voo(Voos.num_voo, arquivo1);
        if (valida == 0)
        {
            // quando retorna 0, foi porque a estrutura estava incorreta
            printf("\nnumero de voo incopativel!\n");
        }
        if (valida == -1)
        {
            // quando retorna -1, foi porque esse numero de voo ja foi resgitrado anteriormente;
            printf("\nnumero de voo ja utilizado!\n");
        }
        
    } while (valida == 0 || valida == -1);

    do
    {
        printf("Digite a quantidade maxima de passageiros: \n");
        scanf(" %d", &Voos.quant_max);
        
        if(Voos.quant_max < 0){
            printf("qunatidade de pessoas invalida! ");
        }
        
    } while (Voos.quant_max <= 0); // a quantidade de passageiros dentro do voo nao pode ser negativa nem igual a 0;

    printf("Digite o dia de partida do voo (dd/mm/aaaa): \n");
    scanf(" %d/%d/%d", &Voos.dia_voo, &Voos.mes_voo, &Voos.ano_voo);

    // Verificando se a data do voo esta correta;
    while (valida_data_voo(Voos.dia_voo, Voos.mes_voo, Voos.ano_voo) == 0)
    {
        // quando retorna 0 foi porque a data digitada nao existe;
        printf("\ndados da data do voo erradas!\n");

        printf("Digite o dia de partida do voo (dd/mm/aaaa): \n");
        scanf(" %d/%d/%d", &Voos.dia_voo, &Voos.mes_voo, &Voos.ano_voo);
    }

    printf("Digite a hora de partida do voo (hh:mm): \n");
    scanf(" %d:%d", &Voos.hora_partida, &Voos.min_partida);

    while (valida_hora_voo(Voos.hora_partida, Voos.min_partida) == 0)
    {
        // quando retorna 0 foi porque a hora digitada nao existe;
        printf("\nhora de voo invalida!\n");

        printf("Digite a hora de partida do voo (hh:mm): \n");
        scanf(" %d:%d", &Voos.hora_partida, &Voos.min_partida);
    }
    
    // ao fim do cadastro o voo vai ser considerado automaticamente ativo;
    
    Voos.status_voo = 'A';
    
    //escrevendo o conteudo do cadastrado no arquivo;
    fwrite(&Voos, 1, sizeof(struct voo), arquivo1);

    // fechar arquivo;
    fclose(arquivo1);
    
    printf("\nvoo cadastrado!\n");
}

void cadastrar_reserva()
{
    // DECLACOES:
    struct reserva Reservas;
    FILE *arquivo2;
    FILE *arquivo1;
    int valida = 1;
    int sexo;
    // INSTRUCOES:

    // abrir arquivo;

    // Se nao tiver um arquivo, ele vai criar automatico;

    arquivo2 = fopen("reservas.bin", "a+b");
    arquivo1 = fopen("voos.bin", "rb");

    // manipulando arquivo;
    clear();

    do
    {
        printf("Digite o numero do voo (JEB-XXXX): \n");
        scanf(" %s", Reservas.num_voo);
        // verificando numeero voo;
        valida = valida_num_voo_reserva(Reservas.num_voo, arquivo1);
        if (valida == 0)
        {
            printf("\nnumero de voo nao cadastrado!\n");
        }
        
    } while (valida == 0); // se o valida for 0, foi porque o voo digitado nao consta registro no arquivo;
    
    do
    {
        printf("Digite o numero da reserva (GB-XXXXXX): \n");
        scanf("%s", Reservas.num_reserva);
        // verificando reserva cadastrada voo;
        valida = valida_numero_reserva(Reservas.num_reserva, arquivo2);
        if (valida == 0)
        {
            // quando retorna 0, foi porque o numero da reserva digitada esta em uma estrutura errada;
            printf("\nnumero de reserva incopativel!\n");
        }
        if (valida == 2)
        {
            // quando retorna 2, foi por que o numero de reserva digitado, ja foi utilizado anteriormente;
            printf("\nnumero de reserva ja utilizado!\n");
        }
        
    } while (valida == 0 || valida == 2);

    printf("Digite o seu CPF (xxx.xxx.xxx-xx): \n");
    scanf("%s", Reservas.cpf);

    // restringindo o cpf:
    while (valida_CPF(Reservas.cpf, Reservas.num_voo, arquivo1, arquivo2) == 0)
    {
        //se o valida voo for igual 0 foi porque o cpf digitado ja tem cadastro no voo ou porque sua estrutura esta errada;
        printf("CPF invalido!");
        printf("Digite o seu CPF (xxx.xxx.xxx-xx): \n");
        scanf("%s", Reservas.cpf);
    }

    printf("Digite o seu nome completo: \n");
    scanf("%s", Reservas.nome_completo);

    // restringindo o nome:
    while (valida_nome(Reservas.nome_completo) == 0)
    {
        // quando retorna 0 foi porque o nome digitado e invalido por conter caracteres incorretos;
        printf("nome invalido!\n");
        printf("Digite o seu nome completo: \n");
        scanf("%s", Reservas.nome_completo);
    }

    do
    {
        getchar();

        flush_in();

        printf("Digite o seu sexo: \n1-Masculino    2-Feminino\n");
        scanf("%d", &sexo);
        //opcao ao usuario de escolher seu sexo;
        switch (sexo)
        {
        case 1:
            // para masculino deve ser digitado 1;
            Reservas.sexo = 'M';
            break;
        case 2:
            // para feminino deve ser digitado 2;
            Reservas.sexo = 'F';
            break;
        default:
            // em caso que nao e digitado nem 1 nem 2 o codigo deve considerar o sexo invalido;
            printf("\nSexo invalido, digite seu sexo novamente!\n");
            sexo = 0;
            break;
        }

    } while (sexo == 0);

    printf("digite o dia do seu nascimento (dd/mm/aaaa): \n");
    scanf(" %d/%d/%d", &Reservas.dia_nascimento, &Reservas.mes_nascimento, &Reservas.ano_nascimento);

    // restringindo o dia de nascimento:
    while (valida_nascimento(Reservas.dia_nascimento, Reservas.mes_nascimento, Reservas.ano_nascimento) == 0)
    {
        // se retorna 0 foi porque a data de nascimento e impossivel de existir;
        printf("data de nascimento invalida, digite sua data de nascimento novamente!");
        printf("digite o dia do seu nascimento (dd/mm/aaaa): \n");
        scanf(" %d/%d/%d", &Reservas.dia_nascimento, &Reservas.mes_nascimento, &Reservas.ano_nascimento);
    }
    
    //ao fim do cadastro da reserva, o cadastro sera considerado automaticamente como ativo;
    Reservas.status_reserva = 'A';

    fwrite(&Reservas, 1, sizeof(struct reserva), arquivo2);

    // fechar arquivo;
    fclose(arquivo1);
    fclose(arquivo2);
    
    printf("\nreserva cadastrada!\n");
}

void consultar_voo()
{
    // DECLACOES:
    struct voo consulta_voo;
    struct voo busca_voo;
    struct reserva consulta_reserva;

    int contador;

    float percent;
    // INSTRUCOES:

    // abrindo arquivo:
    FILE *voos;
    FILE *reservas;

    reservas = fopen("reservas.bin", "rb");
    voos = fopen("voos.bin", "rb");
    
    
    clear();
    // analisando qual voo iremos consultar;
    printf("Digite o numero do voo que deseja consultar(JEB-XXXX): \n");
    scanf("%s", consulta_voo.num_voo);
    // Leitura do numero do voo que devera ser consultado;
    
    contador = -1;
    percent = 0;

    // manipulando o arquivo;
    
    fseek(voos, 0, SEEK_SET);
    while (fread(&busca_voo, sizeof(struct voo), 1, voos))
    {
        if (strcmp(busca_voo.num_voo, consulta_voo.num_voo) == 0)
        { // Comparacao para confirmar se o numero digitado esta no arquivo;
            clear();
            printf("\nVoo Encontrado : %s\n\n", busca_voo.num_voo);
            
            if(busca_voo.status_voo == 'C') // conferindo se o voo esta cancelado;
            {
                contador = -2;
            }
            else
            {
                contador = 0;
                printf("\nReservas ativas:\n");
                
                fseek(reservas, 0, SEEK_SET);
                while (fread(&consulta_reserva, sizeof(struct reserva), 1, reservas))
                {
                    if (strcmp(consulta_reserva.num_voo, busca_voo.num_voo) == 0)
                    {
                        if (consulta_reserva.status_reserva == 'A')
                        { // Se o status da reserva for ativo, apresente-a na tela;
                            printf("\t%s\n", consulta_reserva.num_reserva);
                            // Soma das reservas efetuadas para o voo inserido;
                            contador++;
                        }
                    }
                }
                // calculando o percentual de passageiros no voo;
                percent = (contador * 100) / busca_voo.quant_max; // Calculo do percentual de passageiros com reservas ativas para aquele voo;
                break;
            }
        }
    }
    if (contador > 0)
    { // Saida se o voo tiver reservas;
        printf("\nPercentual de ocupacão: %.3f%%\n", percent);
        printf("Data: %02d/%02d/%04d\nHora: %02d:%02d\nStatus do Voo:%c\n\n", busca_voo.dia_voo, busca_voo.mes_voo, busca_voo.ano_voo, busca_voo.hora_partida, busca_voo.min_partida, busca_voo.status_voo);
    }
    else
    {
        if (contador == 0)
        { // Se o voo não tiver nenhuma reserva ativa;
            printf("\nNao ha reservas ativas!\n\n");
        }
        else if (contador == -1)
        {
            // se o voo nao existir dentro do arquivo;
            printf("\nVoo nao encontrado!\n\n");
        }
        else
        {
            // se o voo consultado estiver cancelado;
            printf("\nEsse voo foi cancelado.\n\n");
        }
    }

    // Fechamento dos arquivos

    fclose(voos);
    fclose(reservas);
}

void consultar_reserva()
{
    // DECLACOES:
    struct reserva consulta_reserva;
    struct reserva busca_reserva;
    struct voo busca_voo;
    int valida = 1;

    FILE *reservas;
    FILE *voos;

    // INSTRUCOES:
    // abrindo arquivo;
    reservas = fopen("reservas.bin", "rb");
    voos = fopen("voos.bin", "rb");

    // manipulando arquivo;

    printf("Digite o numero da reserva que deseja consultar (GB-XXXXXX): \n");
    scanf("%s", consulta_reserva.num_reserva);

    
    // mostrar todos os dados da resrva;
    
    fseek(reservas, 0, SEEK_SET);
    while (fread(&busca_reserva, sizeof(struct reserva), 1, reservas))
    {
        if (strcmp(busca_reserva.num_reserva, consulta_reserva.num_reserva) == 0)
        {
            // quando encontra a reserva dentro do arquivo;
            valida = 0;
            printf("\nVoo no qual a reserva esta cadastrada: %s \n", busca_reserva.num_voo);
            printf("CPF do passageiro: %s \n", busca_reserva.cpf);
            printf("Nome do passageiro: %s \n", busca_reserva.nome_completo);
            printf("Sexo: %c \n", busca_reserva.sexo);
            printf("Data de nascimento do passageiro: %d/%d/%d \n", busca_reserva.dia_nascimento, busca_reserva.mes_nascimento, busca_reserva.ano_nascimento);
            printf("status da reserva: %c \n\n", busca_reserva.status_reserva);
        }
        
    }
    
    if (valida == 1)
    {
        // se nao houver um cadastro com com a reserva digitada;
        printf("\nnumero de reserva nao encontrado. \n\n");
    }

    // fechando arquivo;
    fclose(reservas);
    fclose(voos);
}

void consultar_passageiro()
{
    // DECLACOES:
    struct reserva consulta_passageiro;
    struct reserva busca_cpf;
    struct voo busca_voo_consulta;
    struct reserva busca_reserva_passageiro;
    fpos_t position;

    FILE *reservaa;
    FILE *vooss;

    // INSTRUCOES:
    
    // abrir arquivos;
    reservaa = fopen("reservas.bin", "rb");
    vooss = fopen("voos.bin", "rb");
    
    // manipulando arquivo;

    printf("digite o cpf do passageiro que deseja consultar (xxx.xxx.xxx-xx): \n");
    scanf("%s", consulta_passageiro.cpf);
    // mostrar os dados de todas as reservas feitas por um passageiro;
    
    while (fread(&busca_cpf, sizeof(struct reserva), 1, reservaa))
    {
        if (strcmp(busca_cpf.cpf, consulta_passageiro.cpf) == 0)
        {
            // mostarndo os dados do passageiro;
            printf("\nconsulta do passageiro de cpf: %s \n", busca_cpf.cpf);
            printf("Nome completo do passageiro: %s \n", busca_cpf.nome_completo);
            printf("Sexo do passageiro: %c \n", busca_cpf.sexo);
            printf("Data de nascimento do passageiro: %d/%d/%d \n", busca_cpf.dia_nascimento, busca_cpf.mes_nascimento, busca_cpf.ano_nascimento);
            
            //mostrando a reserva em que aquele passageiro tem reserva;
            
            printf("\nNumero de reserva: %s \n", busca_cpf.num_reserva);
            printf("Status da reserva: %c \n", busca_cpf.status_reserva);
            printf("\nNumero do voo em que reserva se encontra: %s \n", busca_cpf.num_voo);
            
            fseek(vooss, 0, SEEK_SET);

                    while (fread(&busca_voo_consulta, sizeof(struct voo), 1, vooss))
                    {
                        if (strcmp(busca_voo_consulta.num_voo, busca_cpf.num_voo) == 0)
                        {
                            //mostrando os dados do voo de uma determinada reserva em que o passageiro esta cadastrado;
                            printf("Data de partida do voo: %d/%d/%d \n", busca_voo_consulta.dia_voo, busca_voo_consulta.mes_voo, busca_voo_consulta.ano_voo);
                            printf("Hora de partida do voo: %d:%d \n", busca_voo_consulta.hora_partida, busca_voo_consulta.min_partida);
                            printf("Status do voo: %c \n", busca_voo_consulta.status_voo);
                        }
                    }
                 
             
        }
        else
        {
            printf("cpf nao apresenta nenhuma reserva cadastrada!");
        }
    }

    // fechando arquivos;
    fclose(reservaa);
    fclose(vooss);
}

void cancelar_voo()
{
    // DECLARACOES:
    struct voo voo_cancelar;
    struct voo busca_voo_cancelar;
    int validar;

    FILE *voar;
    fpos_t position;

    // INSTRUCOES:

    validar = 0;
    // abrindo arquivo;

    voar = fopen("voos.bin", "r+b");

    // manipulando arquivo;
    do
    {
        printf("Digite o numero do voo que deseja cancelar (JEB-XXXX): \n");
        scanf("%s", voo_cancelar.num_voo);
        // verificando numeero voo;
        validar = valida_cancelar_voo(voo_cancelar.num_voo, voar);
        if (validar == 0)
        {
            printf("\nnumero de voo incopativel! ");
        }
        if (validar == -1)
        {
            printf("\nnumero de voo nao cadastrado! ");
        }
        
    } while (validar == 0 || validar == -1); // conferindo se o voo digitado existe, so pode cancelar voo que realmente existem;
    
    printf("\nVoo que sera cancelado: %s \n", voo_cancelar.num_voo);

    // fazendo a alteração para voo cancelado;
    fseek(voar, 0, SEEK_SET);
    fgetpos(voar, &position);
    
    while (fread(&busca_voo_cancelar, sizeof(struct voo), 1, voar))
    {
        if (strcmp(busca_voo_cancelar.num_voo, voo_cancelar.num_voo) == 0)
        {
            struct voo alterado;
            if (busca_voo_cancelar.status_voo == 'C'){
                
                printf("\nEsse voo já foi cancelado anteriormente!\n\n");
                
            }
            else
            {
                // transferindo os dados do voo cancelado e alterando seu status para 'C'(cancelado);
                
                voo_cancelar.quant_max = busca_voo_cancelar.quant_max;
                voo_cancelar.dia_voo = busca_voo_cancelar.dia_voo;
                voo_cancelar.mes_voo = busca_voo_cancelar.mes_voo;
                voo_cancelar.ano_voo = busca_voo_cancelar.ano_voo;
                voo_cancelar.hora_partida = busca_voo_cancelar.hora_partida;
                voo_cancelar.min_partida = busca_voo_cancelar.min_partida;
                voo_cancelar.status_voo = 'C';
                
                fsetpos(voar, &position);
                fwrite(&voo_cancelar, sizeof(struct voo), 1, voar);
                printf("\nVoo cancelado com sucesso.\n");
                break;
            }
        }
        fgetpos(voar, &position);
    }
    
    //fechando arquivo;
    fclose(voar);
    
}

void cancelar_reserva()
{
    // DECLARACOES:
    struct reserva consulta_reserva;
    struct reserva busca_reserva;
    int validar;
    int valida = 1;

    FILE *reservar;
    FILE *auxiliar_res;
    fpos_t position;

    // INSTRUCOES:

    validar = 0;
    // abrindo arquivo;

    reservar = fopen("reservas.bin", "r+b");
    auxiliar_res = fopen("auxiliarRes.bin", "w+b");

    // manipulando arquivo;
    do
    {
        printf("Digite o numero da reserva que deseja cancelar (GB-XXXXXX): \n");
        scanf("%s", consulta_reserva.num_reserva);
        // verificando reserva cadastrada voo;
        validar = valida_cancelar_reserva(consulta_reserva.num_reserva, reservar);
        if (validar == 0)
        {
            printf("\nnumero de reserva incopativel! \n");
        }
        if (validar == -1)
        {
            printf("\nnumero de reserva ainda nao cadastrado! ");
        }

    } while (validar == 0 || validar == -1); // conferindo se a reserva realmente existe para ser cancelada;
    
    printf("\nReserva que sera cancelada: %s \n", consulta_reserva.num_reserva);
    

    // fazendo a alteração para reserva cancelada;
    
    fseek(reservar, 0, SEEK_SET);
    
    while (fread(&busca_reserva, sizeof(struct reserva), 1, reservar))
    {
        if (strcmp(busca_reserva.num_reserva, consulta_reserva.num_reserva) == 0)
        {
            // transferindoos os dados da reserva a ser cancelada ao arquivo auxiliar com troca do status;
            if (busca_reserva.status_reserva == 'C'){
                
                valida = 1;
                fwrite(&busca_reserva, sizeof(struct reserva), 1, auxiliar_res);
                
            }
            else
            {
                valida = 0;
                busca_reserva.status_reserva = 'C';
                
                fwrite(&busca_reserva, sizeof(struct reserva), 1, auxiliar_res);
            }
            
        }
        else
        {
            // copiando os arquivos que nao devem ser cancelados ao arquivo auxiliar;
            fwrite(&busca_reserva, sizeof(struct reserva), 1, auxiliar_res);
        }
    }
    
    if(valida == 0){
        // caso valida seja igual a 0 foi porque a reserva foi cancelada com exito;
        printf("\nReserva cancelada com sucesso. \n");
    }
    else
    {
        // caso retorne 1 foi porque essa reserva ja foi cancelada anteriormente;
        printf("numero de reserva ja cancelado anteriormente. \n");
    }
    
    //removendo o arquivo principal e renomeando o auxiliar a principal;
    remove("reservas.bin");
    rename("auxiliarRes.bin", "reservas.bin");
    
    fclose(reservar);
    
}

void excluir_voo()
{
    // DECLARACOES
    struct voo reg_voo;
    struct voo voo_excluir;
    struct reserva reg_res;
    int status, renomeia, deleta;
    char resp;
    char numero_voo[TAM_NUM_VOO];

	int fvoo=0;
    int freserva=0;

    FILE *voos;
    FILE *reservas;
    FILE *auxiliar_res;
	FILE *auxiliar_voos;
    // INSTRUCOES

    printf("Digite o numero do voo que deseja excluir: \n");
    scanf("%s", voo_excluir.num_voo);
    //abrindo os arquivos para excluir o voo;
    
    voos = fopen("voos.bin", "r+b");
    reservas = fopen("reservas.bin", "r+b");
    auxiliar_voos = fopen("auxiliarVoo.bin", "w+b");
    auxiliar_res = fopen("auxiliarRes.bin", "w+b");
    
    //manipulando arquivo;
    
    if (voos == NULL)
    {
        printf("\n\nFalha na Abertura do Arquivo: voos.bin");
    }
    else
    {
        while (fread(&reg_voo, sizeof(struct voo), 1, voos))
        {
            if (strcmp(reg_voo.num_voo, voo_excluir.num_voo) == 0)
            {
                // informando se o voo que deseja excluir e o voo com essas informacoes;
                printf("\n\nNumero voo.......:   %s", voo_excluir.num_voo);
                printf("\nData de partida do voo....:   %d/%d/%d", reg_voo.dia_voo, reg_voo.mes_voo, reg_voo.ano_voo);
                printf("\nHora de partida do voo.......:   %d:%d", reg_voo.hora_partida, reg_voo.min_partida);
                printf("\n\nConfirma exclusao (S/N): ");
                flush_in();
                scanf("%c", &resp);
                if ((resp == 'S') || (resp == 's'))
                {
			        fvoo=1;   
                }
            }
            else {
                // passando os dados dos voos ao arquivo auxiliar;
			    fwrite(&reg_voo, sizeof(struct voo), 1, auxiliar_voos);
		    }
            
        }
            
        if((resp == 'S') || (resp == 's')) {
            // se a resposta for sim iremos excluir o voo;
            while(fread(&reg_res, sizeof(struct reserva),1, reservas));
            {
                            if(strcmp(reg_res.num_voo, voo_excluir.num_voo) == 0)
                            {
			                    freserva=1;
                            }
                            else{
                                // escrevendo todos os dados do arquivo de reserva exceto os das reservas que estavam no voo excluido;
                                fwrite(&reg_res, sizeof(struct reserva), 1, auxiliar_res);
                            }
                        }
            fclose(reservas);
	        fclose(auxiliar_res);

        }
        if (!fvoo) {
		        printf("Nenhum voo encontrado: %s\n\n", voo_excluir.num_voo);
	    }
        fclose(voos);
	    fclose(auxiliar_voos);

        if ((resp == 'S') || (resp == 's'))
        {
        // removendo os arquivos principais e renomeando os arquivos auxiliares a principais;
        printf("voo excluido com sucesso!\n");
        remove("voos.bin");
        rename("auxiliarVoo.bin", "voos.bin");
        remove("reservas.bin");
        rename("auxiliarRes.bin", "reservas.bin");
        }
    }
    
    
}

int main()
{
    // DECLACOES:
    int opcao;
    // INSTRUCOES:

    do
    {
        printf("escolha uma opcao:\n\n");

        menu(); // criar as funcoes que o usuario pode escolher;

        scanf("%d", &opcao); // ler qual funcao ele quer;

        switch (opcao) // identificar o que cada opcão vai fazer;
        {
        case 1: // cadastrar o voo;
            cadastrar_voo();
            break;
        case 2: // cadastrar reserva no voo:
            cadastrar_reserva();
            break;
        case 3: // Consultar voo:
            consultar_voo();
            break;
        case 4: // Consultar reserva:
            consultar_reserva();
            break;
        case 5: // Consultar passageiro
            consultar_passageiro();
            break;
        case 6: // Cancelar voo
            cancelar_voo();
            break;
        case 7: // Cancelar reserva
            cancelar_reserva();
            break;
        case 8: // Excluir voo
            excluir_voo();
            break;
        case 9: // Sair do programa
            printf("Programa Encerrado.");
            break;
        default:
            printf("Numero invalido. Digite novamente o que deseja!\n");
        }
    } while (opcao != 9);
}

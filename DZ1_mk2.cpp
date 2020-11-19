#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct Block
{
    std::string block_str = "";
    uint8_t mybytes[12];
};

typedef Block block;

Block operator^(block b1, block b2)
{
    block rez;
    for (int i = 0; i < 12; ++i)
    {
        rez.mybytes[i] = b1.mybytes[i] ^ b2.mybytes[i];
        rez.block_str += rez.mybytes[i];
    }
    return rez;
}

Block operator>>(block b, int n)
{
    block rez;
    uint8_t tmp[12];
    for (int i = 0; i < 12; ++i)
    {
        tmp[i] = b.mybytes[i] << (8 - n);
        rez.mybytes[i] = b.mybytes[i] >> n;
    }
    rez.mybytes[0] = rez.mybytes[0] | tmp[11];
    for (int i = 1; i < 12; ++i)
    {
        rez.mybytes[i] = rez.mybytes[i] | tmp[i - 1];
    }
    for (int i = 0; i < 12; ++i)
    {
        rez.block_str += rez.mybytes[i];
    }
    return rez;
}

Block operator<<(block b, int n)
{
    block rez;
    
    uint8_t tmp[12];
    for (int i = 0; i < 12; ++i)
    {
        tmp[i] = b.mybytes[i] >> (8 - n);
        rez.mybytes[i] = b.mybytes[i] << n;
    }
    rez.mybytes[11] = rez.mybytes[11] | tmp[0];
    for (int i = 0; i < 11; ++i)
    {
        rez.mybytes[i] = rez.mybytes[i] | tmp[i + 1];
    }
    for (int i = 0; i < 12; ++i)
    {
        rez.block_str += rez.mybytes[i];
    }
    return rez;
}

//1 - зашифровать, 0 - расшифровать

int main(int argc, char* argv[])
{
    //создание вектора инициализации для рандома
    std::string vec_in_str = argv[2];
    vec_in_str += '\n';
    int vec_in = 0;
    unsigned int i = 0;
    while (vec_in_str[i] != '\n')
    {
        vec_in += static_cast<int>(vec_in_str[i]) - 48;
        ++i;
        vec_in *= 10;
    }
    vec_in /= 10;
    std::srand(vec_in);
    //-----------------------------------------------------------------
    //создание блоков и гамм
    std::string plaintext;
    std::vector<block> blocks;
    std::vector<block> gammas;
    //-----------------------------------------------------------------
    //вывод аргументов командной строки
    for (int count = 0; count < argc; ++count)
        std::cout << count << " " << argv[count] << std::endl;
    std::ifstream in(argv[3]);
    std::ofstream out(argv[4]);
    //-----------------------------------------------------------------
    unsigned int count_of_blocks = 0;
    std::getline(in, plaintext);

    //plaintext = "kdkkfjnvdjfn  nvdrf8 d ufenur u euoifund suernm jvn4748  67 8   9 8 76 5 \n";
    block current_b;
    i = 0;
    //заполнение символами = для ровного деления
    if (plaintext.length() % 12 == 1) plaintext += "===========";
    else if (plaintext.length() % 12 == 2) plaintext += "==========";
    else if (plaintext.length() % 12 == 3) plaintext += "=========";
    else if (plaintext.length() % 12 == 4) plaintext += "========";
    else if (plaintext.length() % 12 == 5) plaintext += "=======";
    else if (plaintext.length() % 12 == 6) plaintext += "======";
    else if (plaintext.length() % 12 == 7) plaintext += "=====";
    else if (plaintext.length() % 12 == 8) plaintext += "====";
    else if (plaintext.length() % 12 == 9) plaintext += "===";
    else if (plaintext.length() % 12 == 10) plaintext += "==";
    else if (plaintext.length() % 12 == 11) plaintext += "=";
    //-----------------------------------------------------------------
    //делим на блоки по 12
    plaintext += '\n';
    while (plaintext[i] != '\n') {
        current_b.block_str = plaintext.substr(i, 12);
        i += 12;
        blocks.push_back(current_b);
        count_of_blocks++;
    }
    std::cout << count_of_blocks << std::endl;
    for (unsigned int j = 0; j < blocks.size(); ++j) std::cout << blocks[j].block_str << std::endl;
    for (unsigned int j = 0; j < count_of_blocks; ++j)
    {
        for (int k = 0; k < 12; ++k)
        {
            blocks[j].mybytes[k] = static_cast<uint8_t>(blocks[j].block_str[k]);
        }
    }
    
    std::cout << std::endl << "-----------------" << std::endl;
    for (unsigned int j = 0; j < blocks.size(); ++j)
    {
        std::cout << blocks[j].block_str << std::endl;
        for (int k = 0; k < 12; ++k)
        {
            std::cout << int(blocks[j].mybytes[k]) << "|";
        }
        std::cout << std::endl;
    }
    std::cout << "+++++++++++++++++++" << std::endl;
    //----------------------------------------------------------------------
    block current_g;
    //current_g.block_str = "sampletext0";
    for (unsigned int j = 0; j < count_of_blocks; ++j) {
        for (int k = 0; k < 12; ++k)
        {
            current_g.mybytes[k] = rand();
            current_g.block_str += current_g.mybytes[k];
        }
        gammas.push_back(current_g);
        current_g.block_str = "";
    }
    for (unsigned int j = 0; j < gammas.size(); ++j)
    {
        std::cout << gammas[j].block_str << std::endl;
        for (int k = 0; k < 12; ++k)
        {
            std::cout << int(gammas[j].mybytes[k]) << "|";
        }
        std::cout << std::endl;
    }


    
    
    std::vector<block> rez;
    std::cout << argv[1] << std::endl;
    std::string comand = argv[1];
    //std::cout << static_cast<int>(comand[0]) << std::endl; 1 в string это 49 в int
    if (static_cast<int>(comand[0]) == 49)
    {
        std::cout << "-------1---------\n";
        for (unsigned int j = 0; j < count_of_blocks; ++j)
        {
            current_b = blocks[j] ^ gammas[j];
            rez.push_back(current_b);
            rez[j] = rez[j] >> 6;
            
            std::cout << rez[j].block_str << std::endl;
            
            out << rez[j].block_str;
        }
    }
    if (static_cast<int>(comand[0]) == 48)
    {
        std::cout << "---------0-----------\n";
        for (unsigned int j = 0; j < count_of_blocks; ++j)
        {
            current_b = blocks[j] << 6;

            rez.push_back(current_b);
            rez[j] = rez[j] ^ gammas[j];
            
            
            std::cout << rez[j].block_str << std::endl;
            out << rez[j].block_str;
        }
    }
    std::cout << std::endl;
    for (unsigned int j = 0; j < rez.size(); ++j)
    {
        std::cout << rez[j].block_str << std::endl;
        for (int k = 0; k < 12; ++k)
        {
            std::cout << int(rez[j].mybytes[k]) << "|";
        }
        std::cout << std::endl;
    }
    return 0;
}

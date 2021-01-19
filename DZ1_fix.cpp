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
    int vec_in = 0;
    unsigned int i = 0;
    for (char c : vec_in_str)
    {
        vec_in += static_cast<int>(c) - 48;
        ++i;
        vec_in *= 10;
    }
    vec_in /= 10;
    std::srand(vec_in);
    //-----------------------------------------------------------------
    //создание блоков и гамм
    std::string plaintext;
    std::vector<block> blocks;
    //-----------------------------------------------------------------
    //вывод аргументов командной строки
    std::ifstream in(argv[3], std::ios::binary);
    std::ofstream out(argv[4]);
    //-----------------------------------------------------------------
    unsigned int count_of_blocks = 0;
    //std::getline(in, plaintext);
    uint8_t block_for_read[250]; // из файла мы будем читать текст блоками по 250 символов
    std::string str = "";
    while (!in.eof())
    {
        in.read(reinterpret_cast<char*>(&block_for_read), 250); // считали 250 или меньше символов
        plaintext.append(reinterpret_cast<char*>(&block_for_read), in.gcount()); // добавили в плайнтекст столько символов, сколько действительно было считано
    }
    block current_b;

    //заполнение символами = для ровного деления
    std::string add;
    short int add_size = (12 - plaintext.length() % 12) % 12;
    for (int i = 0; i < add_size; ++i)
    {
        add += "=";
    }
    plaintext += add;
    //-----------------------------------------------------------------
    //делим на блоки по 12
    for (int i = 0; i < plaintext.length(); i += 12)
    {
        current_b.block_str = plaintext.substr(i, 12);
        blocks.push_back(current_b);
        count_of_blocks++;
    }
    for (unsigned int j = 0; j < count_of_blocks; ++j)
    {
        for (int k = 0; k < 12; ++k)
        {
            blocks[j].mybytes[k] = static_cast<uint8_t>(blocks[j].block_str[k]);
        }
    }
    //----------------------------------------------------------------------
    std::vector<block> rez;
    std::string comand = argv[1];
    Block current_g;
    if (static_cast<int>(comand[0]) == 49)
    {
        for (unsigned int j = 0; j < count_of_blocks; ++j)
        {
            current_g.block_str = "";
            for (int k = 0; k < 12; ++k)
            {
                current_g.mybytes[k] = rand();
                current_g.block_str += current_g.mybytes[k];
            }
            current_b = blocks[j] ^ current_g;
            rez.push_back(current_b);
            rez[j] = rez[j] >> 6;
            out << rez[j].block_str;
        }
    }
    if (static_cast<int>(comand[0]) == 48)
    {
        std::string rez_out = "";
        for (unsigned int j = 0; j < count_of_blocks; ++j)
        {
            current_g.block_str = "";
            for (int k = 0; k < 12; ++k)
            {
                current_g.mybytes[k] = rand();
                current_g.block_str += current_g.mybytes[k];
            }
            current_b = blocks[j] << 6;

            rez.push_back(current_b);
            rez[j] = rez[j] ^ current_g;
            rez_out += rez[j].block_str;
        }
        while (rez_out[rez_out.length() - 1] == '=')
        {
            rez_out.erase(rez_out.length() - 1, 1);
        }
        out << rez_out;
    }
    
    

    return 0;
}
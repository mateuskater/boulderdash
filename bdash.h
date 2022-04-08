typedef struct jogador{
    int x, y;
}jogador;

typedef struct tile{
    enum tipo{Empty = 0, Wall, Dirt, Player, Rock, Exit, Brick, Diamond}tipo;
    int x, y;
}tile;
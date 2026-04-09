#include "editor_controller.h"

int main() {
    Editor ed;
    create_editor(&ed);
    run_main_menu(&ed);
    return 0;
}
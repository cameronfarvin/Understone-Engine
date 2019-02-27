gcc -g -O0 -Wall -Werror -Wextra -Wshadow -Wcast-align \
    engine.c \
    nix/nix_platform.c \
    engine_tools/ogl_tools.c \
    renderers/triangle_renderer.c \
    data_structures/uDynamicArray.c \
    tests/tests.c \
    -Inix -Iwin -Iengine_tools -Irenderers -I. -Idata_structures -Itests \
    -lX11 -lGL -lGLU -lm -std=c11 -o engine

gcc -g3 -O0 -Wall -Werror -Wextra -Wshadow -Wcast-align -std=c11 -o engine\
    engine.c \
    nix/nix_platform.c \
    engine_tools/ogl_tools.c \
    engine_tools/event_tools.c \
    engine_tools/image_tools.c \
    engine_tools/type_tools.c \
    renderers/triangle_renderer.c \
    data_structures/uDynamicArray.c \
    tests/tests.c \
    -Inix -Iwin -Iengine_tools -Irenderers -I. -Idata_structures -Itests \
    -lX11 -lGL -lGLU -lm

gcc \
    engine.c \
    nix/nix_platform.c \
    engine_tools/ogl_tools.c \
    renderers/triangle_renderer.c \
    -Inix -Iwin -Iengine_tools -Irenderers -I. \
    -g -Wall -Werror -lX11 -lGL -lGLU -std=c11 -o engine

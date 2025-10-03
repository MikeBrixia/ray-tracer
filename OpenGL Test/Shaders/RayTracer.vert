#version 450

void main() {
     // Fullscreen triangle (no VBO), needed to execute frag shader on all pixels
     // and therefore implement ray tracing.
     const vec2 verts[3] = vec2[3](
     vec2(-1.0, -1.0),
     vec2( 3.0, -1.0),
     vec2(-1.0,  3.0)
     );
     
     gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);

}

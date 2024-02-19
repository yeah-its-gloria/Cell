struct VertexInput {
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PixelInput VertexMain(VertexInput input) {
    const PixelInput pixel = {
        input.position,
        input.color
    };

    return pixel;
}

float4 PixelMain(PixelInput input) : SV_TARGET {
    return input.color;
}

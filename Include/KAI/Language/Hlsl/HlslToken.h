#pragma once

#include <KAI/Language/Common/TokenBase.h>

KAI_BEGIN

// A token in the Hlsl language
struct HlslTokenEnumType
{
    enum Enum
    {
        None = 0,
        Whitespace,
        Semi,
        AppendStructuredBuffer,
        Asm,
        AsmFragment,
        Bool,
        Break,
        Buffer,
        ByteAddressBuffer,
        Case,
        Continue,
        Class,
        Column,
        ColumnMajor,
        Compile,
        CompileFragment,
        CompileShader,
        Const,
        ComputeShader,
        ConsumeStructuredBuffered,
        Default,
        DepthStencilState,
        DepthStencilView,
        Discard,
        Do,
        Double,
        DomainShader,
        Dword,
        Else,
        Export,
        Extern,
        False,
        Float,
        For,
        Fxgroup,
        GeometryShader,
        GroupShared,
        Half,
        HullShader,
        If,
        In,
        Inline,
        Inout,
        InputPatch,
        Int,
        Interface,
        Line,
        Lineadj,
        Linear,
        LineStream,
        Matrix,
        Min16Float,
        Min10Float,
        Min16Int,
        Min12Int,
        Min16Uint,
        Namespace,
        Nointerpolation,
        Noperspective,
        Null,
        Out,
        OutputPatch,
        PackOffset,
        Pass,
        PixelFragment,
        PixelShader,
        Point,
        PointStream,
        Precise,
        RasterizerState,
        RenderTargetView,
        Return,
        Register,
        RowMajor,
        RwBuffer,
        RwByteAddressBuffer,
        RwStructuredBuffer,
        RwTexture1D,
        RWTexture1DArray,
        RwTexture2D,
        RwTexture2DArray,
        RwTexture3D,
        Sample,
        Sampler,
        SamplerState,
        SamplerComparisonState,
        Shared,
        Snorm,
        StateBlock,
        StateBlockState,
        String,
        Static,
        Struct,
        Switch,
        StructuredBuffer,
        TBuffer,
        Technique,
        Technique10,
        Technique11,
        Texture,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture2DMS,
        Texture2DMSArray,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        True,
        Typedef,
        Triangle,
        TriangleAdj,
        TriangleStream,
        Uint,
        Uniform,
        Unorm,
        Unsigned,
        VertexShader,
        VertexFragment,
        Void,
        Vector,
        Volatile,
        While,

        // the following are operators, not keywords
        OpenParan,
        CloseParan,
        OpenBrace,
        CloseBrace,
        Equiv,
        NotEquiv,
        Comma,
        Mul,
        MulAssign,
        OpenSquareBracket,
        CloseSquareBracket,
        Assign,
        And,
        BitAnd,
        Not,
        Or,
        BitOr,
        Less,
        LessEquiv,
        Greater,
        GreaterEquiv,
        Tab,
        NewLine,
        Comment,
        Divide,
        DivAssign,
        Increment,
        Decrement,
        Dot,
        Minus,
        MinusAssign,
        Plus,
        PlusAssign,
        Ident,
        Colon,
        DoubleColon,
        ThreeDots,

        Reserved,

        PP_Define,
        PP_Elif,
        PP_Else,
        PP_Endif,
        PP_Error,
        PP_If,
        PP_IfDef,
        PP_IfNotDef,
        PP_Include,
        PP_Line,
        PP_Pragma,
        PP_Undef,

        Float2x2,
        Float3x3,
        Float4x4,
        Double2x2,
        Double3x3,
        Double4x4,

    };

    struct Type : TokenBase<HlslTokenEnumType>
    {
        Type()
        {
        }

        Type(Enum val, const LexerBase& lexer, int ln, Slice slice)
            : TokenBase<HlslTokenEnumType>(val, lexer, ln, slice)
        {
        }
    };

    static const char* ToString(Enum val);
};

typedef HlslTokenEnumType::Type HlslToken;
typedef HlslTokenEnumType HlslTokens;

KAI_END
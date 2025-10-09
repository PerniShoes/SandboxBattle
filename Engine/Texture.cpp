#include "base.h"
#include <iostream>
#include <string>
#include "Texture.h"
#include <print>
#include "FileUtils.h"


Texture::Texture( const std::string& imagePath )
	:m_Id{ }
	,m_Width{ 10.0f }
	,m_Height{ 10.0f }
	,m_CreationOk{ false }
{
	CreateFromImage( imagePath );
}

Texture::Texture( const std::string& text, TTF_Font *pFont, const Color4f& textColor )
	:m_Id{}
	,m_Width{ 10.0f }
	,m_Height{ 10.0f }
	,m_CreationOk{ false }
{
	CreateFromString( text, pFont, textColor );
}

Texture::Texture( const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor )
	:m_Id{}
	,m_Width{ 10.0f }
	,m_Height{ 10.0f }
	,m_CreationOk{ false }
{
	CreateFromString( text, fontPath, ptSize, textColor );
}
Texture::Texture( Texture&& other ) noexcept
	:m_Id{ other.m_Id }
	,m_Width{ other.m_Width }
	,m_Height{ other.m_Height }
	,m_CreationOk{ other.m_CreationOk }
{
	other.m_Id = 0;
	other.m_CreationOk = false;
}

Texture& Texture::operator=( Texture&& other ) noexcept
{
	if (this != &other)// no self assignment
	{

		m_Id = other.m_Id;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_CreationOk = other.m_CreationOk;
		other.m_Id = 0;
		other.m_CreationOk = false;
	}
	return *this;
}

Texture::~Texture()
{
	glDeleteTextures( 1, &m_Id );
}

void Texture::CreateFromImage( const std::string& path )
{
	m_CreationOk = true;

	SDL_Surface* pLoadedSurface = IMG_Load(path.c_str());
	if (pLoadedSurface == nullptr)
	{
		std::cerr << "Texture::CreateFromImage, error when calling IMG_Load: " << SDL_GetError() << std::endl;
		m_CreationOk = false;
		return;
	}
	CreateFromSurface(pLoadedSurface);

	// Free loaded surface
	SDL_FreeSurface(pLoadedSurface);

}

void Texture::CreateFromString( const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor )
{
	m_CreationOk = true;

	// Create font
	TTF_Font *pFont{};
	pFont = TTF_OpenFont( fontPath.c_str( ), ptSize );
	if(pFont == nullptr )
	{
		std::cerr << "Texture::CreateFromString, error when calling TTF_OpenFont: " << TTF_GetError( ) << std::endl;
		m_CreationOk = false;
		return;
	}

	// Create texture using this font and close font afterwards
	CreateFromString( text, pFont, textColor );
	TTF_CloseFont( pFont );
}

void Texture::CreateFromString( const std::string& text, TTF_Font *pFont, const Color4f& color )
{
	m_CreationOk = true;
	if ( pFont == nullptr )
	{
		std::cerr << "Texture::CreateFromString, invalid TTF_Font pointer\n" ;
		m_CreationOk = false;
		return;
	}

	// Render text surface
	SDL_Color textColor{};
	textColor.r = Uint8( color.r * 255 );
	textColor.g = Uint8( color.g * 255 );
	textColor.b = Uint8( color.b * 255 );
	textColor.a = Uint8( color.a * 255 );

	SDL_Surface* pLoadedSurface = TTF_RenderText_Blended( pFont, text.c_str( ), textColor );
	if ( pLoadedSurface == nullptr )
	{
		std::cerr << "Texture::CreateFromString, error when calling TTF_RenderText_Blended: " << TTF_GetError( ) << std::endl;
		m_CreationOk = false;
		return;
	}

	// Copy to video memory
	CreateFromSurface( pLoadedSurface );

	// Free loaded surface
	SDL_FreeSurface( pLoadedSurface );
}

void Texture::CreateFromSurface( SDL_Surface* pSurface )
{
	m_CreationOk = true;

	// CHAOS
	// Handle arbitraty bit depths (other than 24 or 32)
	if (pSurface->format->BytesPerPixel != 3 && pSurface->format->BytesPerPixel != 4)
	{
		// convert any unknown/unsupported format to 32-bit RGBA
		SDL_Surface* converted = SDL_ConvertSurfaceFormat(pSurface,SDL_PIXELFORMAT_RGBA32,0);
		if (!converted)
		{
			std::cerr << "Texture::CreateFromSurface failed to convert surface to RGBA32\n";
			m_CreationOk = false;
			return;
		}
		pSurface = converted; // use the converted surface from now on
	}


	//Get image dimensions
	m_Width = float(pSurface->w);
	m_Height =float( pSurface->h);

	// Get pixel format information and translate to OpenGl format
	GLenum pixelFormat{ GL_RGB };
	switch ( pSurface->format->BytesPerPixel )
	{
	case 3:
		if ( pSurface->format->Rmask == 0x000000ff )
		{
			pixelFormat = GL_RGB;
		}
		else
		{
			pixelFormat = GL_BGR;
		}
		break;
	case 4:
		if ( pSurface->format->Rmask == 0x000000ff )
		{
			pixelFormat = GL_RGBA;
		}
		else
		{
			pixelFormat = GL_BGRA;
		}
		break;
	default:
		std::cerr << "Texture::CreateFromSurface, unknown pixel format, BytesPerPixel: " << pSurface->format->BytesPerPixel << "\nUse 32 bit or 24 bit images.\n";
		m_CreationOk = false;
		return;
	}

	//Generate an array of textures.  We only want one texture (one element array), so trick
	//it by treating "texture" as array of length one.
	glGenTextures(1, &m_Id);

	//Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
	//All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
	glBindTexture(GL_TEXTURE_2D, m_Id);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);
	// check for errors. Can happen if a texture is created while a static pointer is being initialized, even before the call to the main function.
	GLenum e = glGetError();
	if (e != GL_NO_ERROR)
	{
		std::cerr << "Texture::CreateFromSurface, error binding textures, Error id = " << e << '\n';
		std::cerr << "Can happen if a texture is created before performing the initialization code (e.g. a static Texture object).\n";
		std::cerr << "There might be a white rectangle instead of the image.\n";
	}

	// Specify the texture's data.  
	// This function is a bit tricky, and it's hard to find helpful documentation. 
	// A summary:
	//    GL_TEXTURE_2D:    The currently bound 2D texture (i.e. the one we just made)
	//                0:    The mipmap level.  0, since we want to update the base level mipmap image (i.e., the image itself,
	//                         not cached smaller copies)
	//          GL_RGBA:    Specifies the number of color components in the texture.
	//                     This is how OpenGL will store the texture internally (kinda)--
	//                     It's essentially the texture's type.
	//       surface->w:    The width of the texture
	//       surface->h:    The height of the texture
	//                0:    The border.  Don't worry about this if you're just starting.
	//      pixelFormat:    The format that the *data* is in--NOT the texture! 
	// GL_UNSIGNED_BYTE:    The type the data is in.  In SDL, the data is stored as an array of bytes, with each channel
	//                         getting one byte.  This is fairly typical--it means that the image can store, for each channel,
	//                         any value that fits in one byte (so 0 through 255).  These values are to be interpreted as
	//                         *unsigned* values (since 0x00 should be dark and 0xFF should be bright).
	//  surface->pixels:    The actual data.  As above, SDL's array of bytes.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, pixelFormat, GL_UNSIGNED_BYTE, pSurface->pixels );

	// Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
	// *smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
	// each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
	// further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
	// them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
}

void Texture::Draw( const Point2f& dstBottomLeft, const Rectf& srcRect ) const
{
	const float epsilon{ 0.001f };
	if ( !m_CreationOk )
	{
		if (!(srcRect.width > epsilon && srcRect.height > epsilon)) // No srcRect specified
		{
			DrawFilledRect(Rectf{ dstBottomLeft.x, dstBottomLeft.y, m_Width, m_Height });
		}
		else
		{
			DrawFilledRect(Rectf{ dstBottomLeft.x, dstBottomLeft.y, srcRect.width, srcRect.height });
		}
	}
	else
	{
		Rectf dstRect{ dstBottomLeft.x, dstBottomLeft.y, srcRect.width, srcRect.height };
		if (!(srcRect.width > epsilon && srcRect.height > epsilon)) // No srcRect specified
		{
			dstRect.width = m_Width;
			dstRect.height = m_Height;
		}
		Draw( dstRect, srcRect );
	}
}

void Texture::Draw( const Rectf& dstRect, const Rectf& srcRect ) const
{
	const float epsilon{ 0.001f };
	if ( !m_CreationOk )
	{
		DrawFilledRect( dstRect );
		return;
	}

	// Determine texture coordinates using srcRect and default destination width and height
	float textLeft{};
	float textRight{};
	float textTop{};
	float textBottom{};

	float defaultDestWidth{};
	float defaultDestHeight{};
	if ( !( srcRect.width > epsilon && srcRect.height > epsilon) ) // No srcRect specified
	{
		// Use complete texture
		textLeft = 0.0f;
		textRight = 1.0f;
		textTop = 0.0f;
		textBottom = 1.0f;

		defaultDestHeight = m_Height;
		defaultDestWidth = m_Width;
	}
	else // srcRect specified
	{
		// Convert to the range [0.0, 1.0]
		textLeft = srcRect.left / m_Width;
		textRight = ( srcRect.left + srcRect.width ) / m_Width;
		textTop = ( srcRect.bottom - srcRect.height ) / m_Height;
		textBottom = srcRect.bottom / m_Height;

		defaultDestHeight = srcRect.height;
		defaultDestWidth = srcRect.width;
	}

	// Determine vertex coordinates
	float vertexLeft{ dstRect.left };
	float vertexBottom{ dstRect.bottom };
	float vertexRight{};
	float vertexTop{};
	if ( !( dstRect.width > 0.001f && dstRect.height > 0.001f ) ) // If no size specified use default size
	{
		vertexRight = vertexLeft + defaultDestWidth;
		vertexTop = vertexBottom + defaultDestHeight;
	}
	else
	{
		vertexRight = vertexLeft + dstRect.width;
		vertexTop = vertexBottom + dstRect.height;

	}

	// Tell opengl which texture we will use
	glBindTexture( GL_TEXTURE_2D, m_Id );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	// Draw
	glEnable( GL_TEXTURE_2D );
	{
		glBegin( GL_QUADS );
		{
			glTexCoord2f( textLeft, textBottom );
			glVertex2f( vertexLeft, vertexBottom );

			glTexCoord2f( textLeft, textTop );
			glVertex2f( vertexLeft, vertexTop );

			glTexCoord2f( textRight, textTop );
			glVertex2f( vertexRight, vertexTop );

			glTexCoord2f( textRight, textBottom );
			glVertex2f( vertexRight, vertexBottom );
		}
		glEnd( );
	}
	glDisable( GL_TEXTURE_2D );
}

GLuint Texture::shaderProgram = 0;
void Texture::InitShader()
{
	std::string vertSource = ReadFile("../Shaders/simple.vert");
	std::string fragSource = ReadFile("../Shaders/replace_white.frag");

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vertCode = vertSource.c_str();
	const char* fragCode = fragSource.c_str();

	glShaderSource(vertexShader,1,&vertCode,nullptr);
	glCompileShader(vertexShader);
	CheckShaderCompileErrors(vertexShader);

	glShaderSource(fragmentShader,1,&fragCode,nullptr);
	glCompileShader(fragmentShader);
	CheckShaderCompileErrors(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	CheckProgramLinkErrors(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Texture::DrawShade(const Rectf& dstRect,const Rectf& srcRect,std::initializer_list<std::pair<Color4f,Color4f>> colorPairs) const
{
	if (!m_CreationOk)
	{
		DrawFilledRect(dstRect);
		return;
	}

	const float epsilon = 0.001f;

	// --- Texture coordinates ---
	float textLeft{},textRight{},textTop{},textBottom{};
	float defaultDestWidth{},defaultDestHeight{};
	if (!(srcRect.width > epsilon && srcRect.height > epsilon))
	{
		textLeft = 0.0f; textRight = 1.0f;
		textTop = 0.0f; textBottom = 1.0f;
		defaultDestWidth = m_Width;
		defaultDestHeight = m_Height;
	}
	else
	{
		textLeft = srcRect.left / m_Width;
		textRight = (srcRect.left + srcRect.width) / m_Width;
		textTop = (srcRect.bottom - srcRect.height) / m_Height;
		textBottom = srcRect.bottom / m_Height;
		defaultDestWidth = srcRect.width;
		defaultDestHeight = srcRect.height;
	}

	// --- Vertex coordinates ---
	float vertexLeft = dstRect.left;
	float vertexBottom = dstRect.bottom;
	float vertexRight = (dstRect.width > epsilon) ? vertexLeft + dstRect.width : vertexLeft + defaultDestWidth;
	float vertexTop = (dstRect.height > epsilon) ? vertexBottom + dstRect.height : vertexBottom + defaultDestHeight;

	// --- Bind shader ---
	glUseProgram(shaderProgram);

	GLint texLoc = glGetUniformLocation(shaderProgram,"uTexture");
	GLint numLoc = glGetUniformLocation(shaderProgram,"uNumColors");
	GLint targetLoc = glGetUniformLocation(shaderProgram,"uTargetColors");
	GLint replacementLoc = glGetUniformLocation(shaderProgram,"uReplacementColors");

	glUniform1i(texLoc,0); // texture unit 0

	// --- Prepare color arrays ---
	int numColors = std::min((int)colorPairs.size(),8); // max 8
	glUniform1i(numLoc,numColors);

	float targetData[32] = {0};
	float replacementData[32] = {0};

	int i = 0;
	for (const auto& pair : colorPairs)
	{
		if (i >= 8) break;

		targetData[i * 4 + 0] = pair.first.r;
		targetData[i * 4 + 1] = pair.first.g;
		targetData[i * 4 + 2] = pair.first.b;
		targetData[i * 4 + 3] = pair.first.a;

		replacementData[i * 4 + 0] = pair.second.r;
		replacementData[i * 4 + 1] = pair.second.g;
		replacementData[i * 4 + 2] = pair.second.b;
		replacementData[i * 4 + 3] = pair.second.a;

		++i;
	}

	glUniform4fv(targetLoc,numColors,targetData);
	glUniform4fv(replacementLoc,numColors,replacementData);

	// --- Bind texture and draw quad ---
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,m_Id);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glTexCoord2f(textLeft,textBottom);  glVertex2f(vertexLeft,vertexBottom);
	glTexCoord2f(textLeft,textTop);     glVertex2f(vertexLeft,vertexTop);
	glTexCoord2f(textRight,textTop);    glVertex2f(vertexRight,vertexTop);
	glTexCoord2f(textRight,textBottom); glVertex2f(vertexRight,vertexBottom);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glUseProgram(0);
}



float Texture::GetWidth() const
{
	return m_Width;
}

float Texture::GetHeight() const
{
	return m_Height;
}

bool Texture::IsCreationOk( ) const
{
	return m_CreationOk;
}

void Texture::DrawFilledRect(const Rectf& rect) const
{
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	{
		glVertex2f(rect.left, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom);
		glVertex2f(rect.left + rect.width, rect.bottom + rect.height);
		glVertex2f(rect.left , rect.bottom + rect.height);
	}
	glEnd();

}

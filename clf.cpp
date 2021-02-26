#include "clf.h"

// ----------------------------------------------------------------
// - Base clf::Engine Implementation                              -
// ----------------------------------------------------------------
SDL_Renderer* clf::Engine::renderer{ nullptr };
SDL_Window* clf::Engine::window{ nullptr };

int clf::Engine::screenWidth;
int clf::Engine::screenHeight; 

int clf::Engine::ScreenWidth() { return screenWidth; }
float clf::Engine::ScreenWidthF() { return static_cast<float>(screenWidth); }
int clf::Engine::ScreenHeight() { return screenHeight; }
float clf::Engine::ScreenHeightF() { return static_cast<float>(screenHeight); }
int clf::Engine::HalfScreenWidth() { return screenWidth / 2; }
float clf::Engine::HalfScreenWidthF() { return screenWidth / 2.0f; }
int clf::Engine::HalfScreenHeight() { return screenHeight / 2; }
float clf::Engine::HalfScreenHeightF() { return screenHeight / 2.0f; }

SDL_Renderer* clf::Engine::GetRenderer() { return renderer; }

//The core clf::Engine methods and structure
void clf::Engine::OnStart() {  }
void clf::Engine::OnInput(const Uint8* keystates) {  }
void clf::Engine::OnUpdate(float deltaTime) {  }
void clf::Engine::OnRender() {  }
void clf::Engine::OnFinish() {  }

bool clf::Engine::Build(const char* title, size_t screenWidth, size_t screenHeight, int subsystemFlags, int windowFlags, const char* iconPath) {
	if (!Initialize(title, screenWidth, screenHeight, subsystemFlags, windowFlags, iconPath))
		return false;

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	OnStart();

	while (isRunning) {
		auto startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				isRunning = false;
		}

		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		OnInput(keystates);
		OnUpdate(deltaTime);
		OnRender();

		SDL_RenderPresent(renderer);

		auto endTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch().count();

		float elapsed = static_cast<float>((endTime - startTime) * 0.001);
		deltaTime = elapsed;
		deltaTime = (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;
	}

	OnFinish();

	//Unloads the SDL_Mixer library
	Mix_HaltMusic();
	Mix_CloseAudio();
	while (Mix_Init(0))
		Mix_Quit();

	//Unloads all SDL libraries
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();

	return true;
}

//Creates the window and renderer
bool clf::Engine::Initialize(const char* title, size_t screenWidth, size_t screenHeight, int subsystemFlags, int windowFlags, const char* iconPath) {
	if (SDL_Init(subsystemFlags) < 0 || !IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) || TTF_Init() == -1 || !Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
		return false;

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		return false;

	window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screenWidth,
		screenHeight,
		windowFlags);

	if (!window)
		return false;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (!renderer)
		return false;

	//Set Window Icon
	if (iconPath) {
		SDL_Surface* icon{ IMG_Load(iconPath) };
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);
	}

	isRunning = true;

	return true;
}

// ----------------------------------------------------------------
// - clf::Utilities::Info Implementation                                     -
// ----------------------------------------------------------------
int clf::Utilities::Info::GetTextureWidth(SDL_Texture* texture) {
	int width{ 0 };
	SDL_QueryTexture(texture, NULL, NULL, &width, nullptr);

	return width;
}

float clf::Utilities::Info::GetTextureWidthF(SDL_Texture* texture) {
	return static_cast<float>(GetTextureWidth(texture));
}

int clf::Utilities::Info::GetTextureHeight(SDL_Texture* texture) {
	int height{ 0 };
	SDL_QueryTexture(texture, NULL, NULL, nullptr, &height);

	return height;
}

float clf::Utilities::Info::GetTextureHeightF(SDL_Texture* texture) {
	return static_cast<float>(GetTextureHeight(texture));
}

SDL_Rect clf::Utilities::Info::GetRectCenter(const SDL_Rect& topLeft) {
	return { topLeft.x + topLeft.w / 2, topLeft.y + topLeft.h / 2, topLeft.w, topLeft.h };
}

SDL_FRect clf::Utilities::Info::GetRectCenterF(const SDL_FRect& topLeft) {
	return { topLeft.x + topLeft.w / 2.0f, topLeft.y + topLeft.h / 2.0f, topLeft.w, topLeft.h };
}

SDL_Rect clf::Utilities::Info::GetRectTopLeft(const SDL_Rect& center) {
	return { center.x - center.w / 2, center.y - center.h / 2, center.w, center.h };
}

SDL_FRect clf::Utilities::Info::GetRectTopLeftF(const SDL_FRect& center) {
	return { center.x - center.w / 2.0f, center.y - center.h / 2.0f, center.w, center.h };
}

SDL_Point clf::Utilities::Info::GetPointCenter(const SDL_Point& topLeft, int size) {
	return { topLeft.x + size / 2, topLeft.y + size / 2 };
}

SDL_FPoint clf::Utilities::Info::GetPointCenterF(const SDL_FPoint& topLeft, float size) {
	return { topLeft.x + size / 2.0f, topLeft.y + size / 2.0f };
}

SDL_Point clf::Utilities::Info::GetPointTopLeft(const SDL_Point& center, int size) {
	return { center.x - size / 2, center.y - size / 2 };
}

SDL_FPoint clf::Utilities::Info::GetPointTopLeftF(const SDL_FPoint& center, float size) {
	return { center.x - size / 2.0f, center.y - size / 2.0f };
}

// ----------------------------------------------------------------
// - clf::Utilities::Sprite Implementation                                   -
// ----------------------------------------------------------------
//Constructor
clf::Utilities::Sprite::Sprite() : 
	source{ 0, 0, 0, 0 },
	destination{ 0.0f, 0.0f, 0.0f, 0.0f },
	texture{ nullptr },
	flip{ SDL_FLIP_NONE },
	angle{ 0.0f } {

}

//Methods
void clf::Utilities::Sprite::OnStart(
	int srcX, int srcY, int srcW, int srcH,
	float dstX, float dstY, float dstW, float dstH,
	const char* filepath, const SDL_RendererFlip& flip, double angle) {

	source = { srcX, srcY, srcW, srcH };
	destination = { dstX, dstY, dstW, dstH };
	texture = filepath ? clf::Asset::LoadSprite(filepath) : nullptr;
	this->angle = angle;
	this->flip = flip;
}

void clf::Utilities::Sprite::Draw() {
	clf::Render::DrawSpriteRotCenterF(texture, source, destination, angle, flip);
}

void clf::Utilities::Sprite::OnFinish() {
	clf::Asset::FreeTexture(texture);
}

//Getters and Setters
//Source Member
const SDL_Rect& clf::Utilities::Sprite::GetSrc() const { return source; }
int clf::Utilities::Sprite::GetSrcX() const { return source.x; }
int clf::Utilities::Sprite::GetSrcY() const { return source.y; };
int clf::Utilities::Sprite::GetSrcW() const { return source.w; };
int clf::Utilities::Sprite::GetSrcH() const { return source.h; };

void clf::Utilities::Sprite::SetSrc(int sourceX, int sourceY, int sourceW, int sourceH) {
	source.x = sourceX;
	source.y = sourceY;
	source.w = sourceW;
	source.h = sourceH;
}

void clf::Utilities::Sprite::SetSrcX(int sourceX) { source.x = sourceX; }
void clf::Utilities::Sprite::SetSrcY(int sourceY) { source.y = sourceY; };
void clf::Utilities::Sprite::SetSrcW(int sourceW) { source.w = sourceW; };
void clf::Utilities::Sprite::SetSrcH(int sourceH) { source.h = sourceH; };

//Destination Member
const SDL_FRect& clf::Utilities::Sprite::GetDst() const { return destination; }
float clf::Utilities::Sprite::GetDstX() const { return destination.x; }
float clf::Utilities::Sprite::GetDstY() const { return destination.y; }
float clf::Utilities::Sprite::GetDstW() const { return destination.w; }
float clf::Utilities::Sprite::GetDstH() const { return destination.h; }

void clf::Utilities::Sprite::SetDst(float destinationX, float destinationY, float destinationW, float destinationH) {
	destination.x = destinationX;
	destination.y = destinationY;
	destination.w = destinationW;
	destination.h = destinationH;
}

void clf::Utilities::Sprite::SetDstX(float destinationX) { destination.x = destinationX; }
void clf::Utilities::Sprite::SetDstY(float destinationY) { destination.y = destinationY; }
void clf::Utilities::Sprite::SetDstW(float destinationW) { destination.w = destinationW; }
void clf::Utilities::Sprite::SetDstH(float destinationH) { destination.h = destinationH; }

//Texture Member
//Be Careful with THIS!!
SDL_Texture* clf::Utilities::Sprite::GetTexture() const { return texture; }

void clf::Utilities::Sprite::SetTexture(SDL_Texture* texture) { 
	if (this->texture)
		clf::Asset::FreeTexture(this->texture);

	this->texture = texture;
}

//Flip Member
void clf::Utilities::Sprite::SetFlip(const SDL_RendererFlip& flip) { this->flip = flip; }

//Angle Member
double clf::Utilities::Sprite::GetAngle() const { return angle; }
void clf::Utilities::Sprite::SetAngle(double angle) { this->angle = angle; }

// ----------------------------------------------------------------
// - clf::Utilities::Text Implementation                                     -
// ----------------------------------------------------------------
//Constructor
clf::Utilities::Text::Text() :
	texture{ nullptr },
	text{ nullptr },
	filepath{ nullptr },
	destination{ 0.0f, 0.0f, 0.0f, 0.0f },
	color{},
	size{ 0 }, outline{ 0 }, style{ 0 } {

}

//Methods
void clf::Utilities::Text::OnStart(float x, float y, const char* filepath, const char* text, int size, const SDL_Color& color, int outline, int style) {
	this->text = text;
	this->size = size;
	this->color = color;
	this->outline = outline;
	this->style = style;
	this->filepath = filepath;
	texture = clf::Asset::LoadText(filepath, size, text, color, outline, style);
	destination = { x, y, clf::Utilities::Info::GetTextureWidthF(texture), clf::Utilities::Info::GetTextureHeightF(texture) };
}

void clf::Utilities::Text::Draw() {
	clf::Render::DrawTextCenterF(texture, destination);
}

void clf::Utilities::Text::OnFinish() {
	clf::Asset::FreeTexture(texture);
}

void clf::Utilities::Text::ReloadTexture() {
	if (this->texture)
		clf::Asset::FreeTexture(texture);

	texture = clf::Asset::LoadText(filepath, size, text, color, outline, style);
}

//Getters and Setters
//Texture Member
//Be Careful with THIS!!
SDL_Texture* clf::Utilities::Text::GetTexture() const { return texture; }
void clf::Utilities::Text::SetTexture(SDL_Texture* texture) { 
	if (this->texture)
		clf::Asset::FreeTexture(texture);

	this->texture = texture; 
}

//Text Member
const char* clf::Utilities::Text::GetText() const { return text; }

void clf::Utilities::Text::SetText(const char* text) { 
	if (this->texture)
		clf::Asset::FreeTexture(this->texture);

	this->text = text;
	ReloadTexture();
	destination.w = clf::Utilities::Info::GetTextureWidthF(texture);
	destination.h = clf::Utilities::Info::GetTextureHeightF(texture);
}

//Filepath Member
const char* clf::Utilities::Text::GetFilepath() const { return filepath; }

void clf::Utilities::Text::SetFilepath(const char* filepath) { 
	if (strcmp(this->filepath, filepath) == 0)
		return;

	this->filepath = filepath; 

	ReloadTexture();
}

//Destination Member
const SDL_FRect& clf::Utilities::Text::GetDst() const { return destination; }
float clf::Utilities::Text::GetDstX() const { return destination.x; }
float clf::Utilities::Text::GetDstY() const { return destination.y; }
float clf::Utilities::Text::GetDstW() const { return destination.w; }
float clf::Utilities::Text::GetDstH() const { return destination.h; }

void clf::Utilities::Text::SetDst(float destinationX, float destinationY, float destinationW, float destinationH) {
	destination.x = destinationX;
	destination.y = destinationY;
	destination.w = destinationW;
	destination.h = destinationH;
}

void clf::Utilities::Text::SetDstX(float destinationX) { destination.x = destinationX; }
void clf::Utilities::Text::SetDstY(float destinationY) { destination.y = destinationY; }
void clf::Utilities::Text::SetDstW(float destinationW) { destination.w = destinationW; }
void clf::Utilities::Text::SetDstH(float destinationH) { destination.h = destinationH; }

//Color Member
const SDL_Color& clf::Utilities::Text::GetColor() const { return color; }

void clf::Utilities::Text::SetColor(const SDL_Color& color) {
	if (this->color.r == color.r && this->color.g == color.g && this->color.b == color.b && this->color.a == color.a)
		return;

	this->color = color;
	ReloadTexture();
}

//Font Style Members
int clf::Utilities::Text::GetSize() const { return size; }

void clf::Utilities::Text::SetSize(int size) {
	if (this->size == size)
		return;

	this->size = size;

	ReloadTexture();
	destination.w = clf::Utilities::Info::GetTextureWidthF(texture);
	destination.h = clf::Utilities::Info::GetTextureHeightF(texture);
}

int clf::Utilities::Text::GetOutline() const { return outline; }

void clf::Utilities::Text::SetOutline(int outline) {
	if (this->outline == outline)
		return;

	this->outline = outline;
	ReloadTexture();
}

int clf::Utilities::Text::GetStyle() const { return style; }

void clf::Utilities::Text::SetStyle(int style) {
	if (this->style == style)
		return;

	this->style = style;
	ReloadTexture();
	destination.w = clf::Utilities::Info::GetTextureWidthF(texture);
	destination.h = clf::Utilities::Info::GetTextureHeightF(texture);
}

// ----------------------------------------------------------------
// - Sfx Implementation                                           - 
// ----------------------------------------------------------------
//Constructor
clf::Utilities::Sfx::Sfx() :
	sound{ nullptr },
	filepath{ nullptr },
	channel{ -1 },
	volume{ 128 } {

}

//Methods
void clf::Utilities::Sfx::OnStart(const char* filepath, int channel, size_t volume) {
	this->filepath = filepath;
	this->channel = channel;
	this->volume = volume;

	sound = clf::Asset::LoadSound(filepath);
}

void clf::Utilities::Sfx::OnFinish() {
	clf::Asset::FreeSound(sound);
}

//Getters and Setters
Mix_Chunk* clf::Utilities::Sfx::GetSound() const { return sound; }

void clf::Utilities::Sfx::SetSound(Mix_Chunk* sound) { 
	if (sound)
		clf::Asset::FreeSound(sound);

	this->sound = sound;
}

//Filepath Member
const char* clf::Utilities::Sfx::GetFilepath() const { return filepath; }

void clf::Utilities::Sfx::SetFilepath(const char* filepath) {
	if (strcmp(this->filepath, filepath) == 0)
		return;

	this->filepath = filepath;

	if (sound)
		clf::Asset::FreeSound(sound);

	sound = clf::Asset::LoadSound(filepath);
}

//Channel Member
int clf::Utilities::Sfx::GetChannel() const { return channel; }
void clf::Utilities::Sfx::SetChannel(int channel) { this->channel = channel; }

//Volume Member
size_t clf::Utilities::Sfx::GetVolume() const { return volume; }
void clf::Utilities::Sfx::SetVolume(size_t volume) { this->volume = volume; }

// ----------------------------------------------------------------
// - Music Implementation                                         - 
// ----------------------------------------------------------------
clf::Utilities::Music::Music() :
	music{ nullptr },
	filepath{ nullptr } {

}

//Methods
void clf::Utilities::Music::OnStart(const char* filepath) {
	this->filepath = filepath;
	music = clf::Asset::LoadMusic(filepath);
}

void clf::Utilities::Music::OnFinish() {
	clf::Asset::FreeMusic(music);
}

//Getters and Setters
//Music Member
Mix_Music* clf::Utilities::Music::GetMusic() const { return music; }
void clf::Utilities::Music::SetMusic(Mix_Music* music) {
	if (this->music)
		clf::Asset::FreeMusic(this->music);

	this->music = music;
}

//Filepath Member
const char* clf::Utilities::Music::GetFilepath() const { return filepath; }

void clf::Utilities::Music::SetFilepath(const char* filepath) {
	if (strcmp(this->filepath, filepath) == 0)
		return;

	if (this->music)
		clf::Asset::FreeMusic(this->music);

	this->filepath = filepath;
	music = clf::Asset::LoadMusic(filepath);
}

// ----------------------------------------------------------------
// - clf::Asset Implementation                                    -
// ----------------------------------------------------------------
SDL_Texture* clf::Asset::LoadSprite(const char* filepath) {
	//Fails if the clf::Asset is not PNG or JPG
	SDL_RWops* rwop{ SDL_RWFromFile(filepath, "rb") };
	if (!(IMG_isPNG(rwop) || IMG_isJPG(rwop)))
		return nullptr;

	SDL_Surface* temp{ IMG_Load(filepath) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::GetRenderer(), temp) };

	SDL_FreeSurface(temp);
	temp = nullptr;
	SDL_RWclose(rwop);
	rwop = nullptr; 

	return texture;
}

SDL_Texture* clf::Asset::LoadText(const char* filepath, int size, const char* text, const SDL_Color& color, int outline, int style) {
	TTF_Font* font{ TTF_OpenFont(filepath, size) };
	TTF_SetFontOutline(font, outline);
	TTF_SetFontStyle(font, style);
	SDL_Surface* temp{ TTF_RenderText_Blended(font, text, color) };
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(clf::Engine::GetRenderer(), temp) };

	SDL_FreeSurface(temp);
	temp = nullptr;
	TTF_CloseFont(font);
	font = nullptr;

	return texture;
}

SDL_Texture* clf::Asset::ChangeText(SDL_Texture* texture, const char* filepath, int size, const char* text, const SDL_Color& color, int outline, int style) {
	SDL_DestroyTexture(texture);
	return LoadText(filepath, size, text, color, outline, style);
}

Mix_Music* clf::Asset::LoadMusic(const char* filepath) {
	Mix_Music* music{ Mix_LoadMUS(filepath) };
	return music;
}

Mix_Chunk* clf::Asset::LoadSound(const char* filepath) {
	Mix_Chunk* sound{ Mix_LoadWAV(filepath) };
	return sound;
}

void clf::Asset::FreeTexture(SDL_Texture* texture) {
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void clf::Asset::FreeMusic(Mix_Music* music) {
	Mix_FreeMusic(music);
	music = nullptr;
}

void clf::Asset::FreeSound(Mix_Chunk* sound) {
	Mix_FreeChunk(sound);
	sound = nullptr;
}

void clf::Asset::FreeChannel(size_t channel) {
	Mix_HaltChannel(static_cast<int>(channel));
}

// ----------------------------------------------------------------
// - Render Implementation                                        -
// ----------------------------------------------------------------
void clf::Render::Clear(const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderClear(clf::Engine::GetRenderer());
}

void clf::Render::DrawPixel(int topLeftX, int topLeftY, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(clf::Engine::GetRenderer(), topLeftX, topLeftY);
}

void clf::Render::DrawPixelF(float topLeftX, float topLeftY, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawPointF(clf::Engine::GetRenderer(), topLeftX, topLeftY);
}

void clf::Render::DrawPixelCenter(int centerX, int centerY, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(clf::Engine::GetRenderer(), (clf::Engine::ScreenWidth() / 2) + centerX, (clf::Engine::ScreenHeight() / 2) - centerY);
}

void clf::Render::DrawPixelCenterF(float centerX, float centerY, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawPointF(clf::Engine::GetRenderer(), (clf::Engine::ScreenWidthF() / 2.0f) + centerX, (clf::Engine::ScreenHeightF() / 2.0f) - centerY);
}

void clf::Render::DrawLine(const SDL_Point& start, const SDL_Point& end, const SDL_Color& color) {
	SDL_RenderDrawLine(clf::Engine::GetRenderer(), start.x, start.y, end.x, end.y);
}

void clf::Render::DrawLineF(const SDL_FPoint& start, const SDL_FPoint& end, const SDL_Color& color) {
	SDL_RenderDrawLineF(clf::Engine::GetRenderer(), start.x, start.y, end.x, end.y);
}

void clf::Render::DrawFillRect(const SDL_Rect& topLeft, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(clf::Engine::GetRenderer(), &topLeft);
}

void clf::Render::DrawFillRectF(const SDL_FRect& topLeft, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRectF(clf::Engine::GetRenderer(), &topLeft);
}

void clf::Render::DrawFillRectCenter(const SDL_Rect& center, const SDL_Color& color) {
	DrawFillRect(clf::Utilities::Info::GetRectTopLeft(center), color);
}

void clf::Render::DrawFillRectCenterF(const SDL_FRect& center, const SDL_Color& color) {
	DrawFillRectF(clf::Utilities::Info::GetRectTopLeftF(center), color);
}

void clf::Render::DrawRect(const SDL_Rect& topLeft, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(clf::Engine::GetRenderer(), &topLeft);
}

void clf::Render::DrawRectF(const SDL_FRect& topLeft, const SDL_Color& color) {
	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawRectF(clf::Engine::GetRenderer(), &topLeft);
}

void clf::Render::DrawRectCenter(const SDL_Rect& center, const SDL_Color& color) {
	DrawRect(clf::Utilities::Info::GetRectTopLeft(center), color);
}

void clf::Render::DrawRectCenterF(const SDL_FRect& center, const SDL_Color& color) {
	DrawRectF(clf::Utilities::Info::GetRectTopLeftF(center), color);
}

void clf::Render::DrawCircle(const SDL_Point& topLeft, int radius, const SDL_Color& color) {
	DrawCircleCenter(clf::Utilities::Info::GetPointCenter(topLeft, radius * 2), radius, color);
}

void clf::Render::DrawCircleF(const SDL_FPoint& topLeft, float radius, const SDL_Color& color) {
	DrawCircleCenterF(clf::Utilities::Info::GetPointCenterF(topLeft, radius * 2.0f), radius, color);
}

void clf::Render::DrawCircleCenter(const SDL_Point& center, int radius, const SDL_Color& color) {
	SDL_FPoint p{ static_cast<float>(center.x), static_cast<float>(center.y) };
	DrawCircleCenterF(p, static_cast<float>(radius), color);
}

void clf::Render::DrawCircleCenterF(const SDL_FPoint& center, float radius, const SDL_Color& color) {
	const double PI = 3.1415926535;
	double x1{ 0.0 }, y1{ 0.0 };

	for (double angle{ 0.0 }; angle < 360.0; angle += 0.1) {
		x1 = radius * cos(angle * PI / 180.0);
		y1 = radius * sin(angle * PI / 180.0);

		SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
		SDL_RenderDrawPointF(clf::Engine::GetRenderer(), static_cast<float>(center.x + x1), static_cast<float>(center.y + y1));
	}
}

void clf::Render::DrawFillCircle(const SDL_Point& topLeft, int radius, const SDL_Color& color) {
	SDL_FPoint p{ static_cast<float>(topLeft.x), static_cast<float>(topLeft.y) };
	DrawFillCircleF(p, static_cast<float>(radius), color);
}

void clf::Render::DrawFillCircleF(const SDL_FPoint& topLeft, float radius, const SDL_Color& color) {
	float maxX{ topLeft.x + (radius * 2.0f) - 1.0f }, maxY{ topLeft.y + (radius * 2.0f) - 1.0f };
	float squaredRadius{ radius * radius };
	float cX{ topLeft.x + radius }, cY{ topLeft.y + radius };

	for (float y{ topLeft.y + 1.0f }; y <= maxY; ++y) {
		for (float x{ topLeft.x + 1.0f }; x <= maxX; ++x) {
			float distance = ((x - cX) * (x - cX)) + ((y - cY) * (y - cY));
			if (distance <= squaredRadius) {
				SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
				SDL_RenderDrawPointF(clf::Engine::GetRenderer(), x - 1.0f, y - 1.0f);
			}
		}
	}
}

void clf::Render::DrawFillCircleCenter(const SDL_Point& center, int radius, const SDL_Color& color) {
	DrawFillCircle(clf::Utilities::Info::GetPointTopLeft(center, radius * 2), radius, color);
}

void clf::Render::DrawFillCircleCenterF(const SDL_FPoint& center, float radius, const SDL_Color& color) {
	DrawFillCircleF(clf::Utilities::Info::GetPointTopLeftF(center, radius * 2.0f), radius, color);
}

void clf::Render::DrawTriangle(const SDL_Point& v1, const SDL_Point& v2, const SDL_Point& v3, const SDL_Color& color) {
	SDL_Point vertices[4] = { v1, v2, v3, v1 };

	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawLines(clf::Engine::GetRenderer(), vertices, 4);
}

void clf::Render::DrawTriangleF(const SDL_FPoint& v1, const SDL_FPoint& v2, const SDL_FPoint& v3, const SDL_Color& color) {
	SDL_FPoint vertices[4] = { v1, v2, v3, v1 };

	SDL_SetRenderDrawColor(clf::Engine::GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderDrawLinesF(clf::Engine::GetRenderer(), vertices, 4);
}

void clf::Render::DrawTriangleCenter(const SDL_Point& center, int distance, const SDL_Color& color) {
	int d = distance - 1;
	SDL_Point p1 = { center.x, center.y - d },
			  p2 = { center.x - d, center.y + d },
			  p3 = { center.x + d, center.y + d };
	DrawTriangle(p1, p2, p3, color);
}

void clf::Render::DrawTriangleCenterF(const SDL_FPoint& center, float distance, const SDL_Color& color) {
	float d = distance - 1;
	SDL_FPoint p1 = { center.x, center.y - d },
		p2 = { center.x - d, center.y + d },
		p3 = { center.x + d, center.y + d };
	DrawTriangleF(p1, p2, p3, color);
}

void clf::Render::DrawSprite(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& topLeft) {
	DrawSpriteRot(texture, source, topLeft, 0.0, SDL_FLIP_NONE);
}

void clf::Render::DrawSpriteF(SDL_Texture* texture, const SDL_Rect& source, const SDL_FRect& topLeft) {
	DrawSpriteRotF(texture, source, topLeft, 0.0, SDL_FLIP_NONE);
}

void clf::Render::DrawSpriteCenter(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& center) {
	DrawSprite(texture, source, clf::Utilities::Info::GetRectTopLeft(center));
}

void clf::Render::DrawSpriteCenterF(SDL_Texture* texture, const SDL_Rect& source, const SDL_FRect& center) {
	DrawSpriteF(texture, source, clf::Utilities::Info::GetRectTopLeftF(center));
}

void clf::Render::DrawSpriteRot(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& topLeft, const double angle, const SDL_RendererFlip& flip) {
	SDL_RenderCopyEx(clf::Engine::GetRenderer(), texture, &source, &topLeft, angle, nullptr, flip);
}

void clf::Render::DrawSpriteRotF(SDL_Texture* texture, const SDL_Rect& source, const SDL_FRect& topLeft, const double angle, const SDL_RendererFlip& flip) {
	SDL_RenderCopyExF(clf::Engine::GetRenderer(), texture, &source, &topLeft, angle, nullptr, flip);
}

void clf::Render::DrawSpriteRotCenter(SDL_Texture* texture, const SDL_Rect& source, const SDL_Rect& center, const double angle, const SDL_RendererFlip& flip) {
	DrawSpriteRot(texture, source, clf::Utilities::Info::GetRectTopLeft(center), angle, flip);
}

void clf::Render::DrawSpriteRotCenterF(SDL_Texture* texture, const SDL_Rect& source, const SDL_FRect& center, const double angle, const SDL_RendererFlip& flip) {
	DrawSpriteRotF(texture, source, clf::Utilities::Info::GetRectTopLeftF(center), angle, flip);
}

void clf::Render::DrawText(SDL_Texture* texture, const SDL_Rect& topLeft) {
	SDL_RenderCopy(clf::Engine::GetRenderer(), texture, nullptr, &topLeft);
}

void clf::Render::DrawTextF(SDL_Texture* texture, const SDL_FRect& topLeft) {
	SDL_RenderCopyF(clf::Engine::GetRenderer(), texture, nullptr, &topLeft);
}

void clf::Render::DrawTextCenter(SDL_Texture* texture, const SDL_Rect& center) {
	DrawText(texture, clf::Utilities::Info::GetRectTopLeft(center));
}

void clf::Render::DrawTextCenterF(SDL_Texture* texture, const SDL_FRect& center) {
	DrawTextF(texture, clf::Utilities::Info::GetRectTopLeftF(center));
}

void clf::Render::DrawTextRot(SDL_Texture* texture, const SDL_Rect& topLeft, const double angle, const SDL_RendererFlip& flip) {
	SDL_RenderCopyEx(clf::Engine::GetRenderer(), texture, nullptr, &topLeft, angle, nullptr, flip);
}

void clf::Render::DrawTextRotF(SDL_Texture* texture, const SDL_FRect& topLeft, const double angle, const SDL_RendererFlip& flip) {
	SDL_RenderCopyExF(clf::Engine::GetRenderer(), texture, nullptr, &topLeft, angle, nullptr, flip);
}

void clf::Render::DrawTextRotCenter(SDL_Texture* texture, const SDL_Rect& center, const double angle, const SDL_RendererFlip& flip) {
	DrawTextRot(texture, clf::Utilities::Info::GetRectTopLeft(center), angle, flip);
}

void clf::Render::DrawTextRotCenterF(SDL_Texture* texture, const SDL_FRect& center, const double angle, const SDL_RendererFlip& flip) {
	DrawTextRotF(texture, clf::Utilities::Info::GetRectTopLeftF(center), angle, flip);
}

// ----------------------------------------------------------------
// - Sound Implementation		                                  -
// ----------------------------------------------------------------
//Music Methods
bool clf::Sound::IsPlayingMusic() {
	return Mix_PlayingMusic();
}

int clf::Sound::GetMusicVolume() {
	return Mix_VolumeMusic(-1);
}

void clf::Sound::SetMusicVolume(size_t volume) {
	Mix_VolumeMusic(static_cast<int>(volume));
}

void clf::Sound::PauseMusic() {
	if (IsPlayingMusic())
		Mix_PauseMusic();
}

void clf::Sound::ResumeMusic() {
	Mix_ResumeMusic();
}

void clf::Sound::PlayMusic(Mix_Music* music, int repeat) {
	Mix_PlayMusic(music, repeat);
}

void clf::Sound::PlayFadeInMusic(Mix_Music* music, int repeat, size_t miliseconds) {
	Mix_FadeInMusic(music, repeat, static_cast<int>(miliseconds));
}

void clf::Sound::FadeOutMusic(size_t miliseconds) {
	while (!Mix_FadeOutMusic(static_cast<int>(miliseconds)) && IsPlayingMusic()) {
		SDL_Delay(100);
	}
}

void clf::Sound::ChangeMusic(Mix_Music* newMusic, int repeat) {
	Mix_HaltMusic();
	PlayMusic(newMusic, repeat);
}

void clf::Sound::ChangeFadeInMusic(Mix_Music* newMusic, int repeat, size_t miliseconds) {
	Mix_HaltMusic();
	PlayFadeInMusic(newMusic, repeat, miliseconds);
}

void clf::Sound::ChangeFadeOutMusic(Mix_Music* newMusic, int repeat, size_t miliseconds) {
	FadeOutMusic(miliseconds);
	PlayMusic(newMusic, repeat);
}

void clf::Sound::ChangeFadeOutFadeInMusic(Mix_Music* newMusic, int repeat, size_t inMS, size_t outMS) {
	FadeOutMusic(outMS);
	PlayFadeInMusic(newMusic, repeat, inMS);
}

//Channel Methods
bool clf::Sound::IsPlayingChannel(int channel) {
	return Mix_Playing(channel);
}

int clf::Sound::GetChannelVolume(int channel) {
	return Mix_Volume(channel, -1);
}

void clf::Sound::SetChannelVolume(int channel, size_t volume) {
	Mix_Volume(channel, static_cast<int>(volume));
}

void clf::Sound::PauseChannel(int channel) {
	Mix_Pause(channel);
}

void clf::Sound::ResumeChannel(int channel) {
	Mix_Resume(channel);
}

void clf::Sound::PlayChannel(int channel, Mix_Chunk* sound, int repeat) {
	Mix_PlayChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat);
}

void clf::Sound::PlayFadeInChannel(int channel, Mix_Chunk* sound, int repeat, size_t miliseconds) {
	Mix_HaltChannel(static_cast<int>(channel));
	Mix_FadeInChannel(channel, sound, repeat > 0 ? repeat - 1 : repeat, miliseconds);
}

void clf::Sound::FadeOutChannel(int channel, size_t miliseconds) {
	Mix_FadeOutChannel(channel, static_cast<int>(miliseconds));
}
#pragma once
#include <vector>
namespace TWC
{
class Hint
{
  public:
    Hint() = default;
    virtual ~Hint() = default;
    virtual void Activate() = 0;

    virtual void Update(std::vector<std::tuple<uint32_t, uint32_t>> ids)
    {
        incomplete = std::move(ids);
    }

  protected:
    std::vector<std::tuple<uint32_t, uint32_t>> incomplete;
};

class MapCinematicHint : public Hint
{
  public:
    ~MapCinematicHint() = default;

    void Activate() override;
};

class MapRecenterHint : public Hint
{
  public:
    ~MapRecenterHint() = default;
    void Activate() override;
    void Update(std::vector<std::tuple<uint32_t, uint32_t>> ids) override
    {
        Hint::Update(std::move(ids));
        last = incomplete.begin();
    }

  private:
    void ActivateMapRecenter(uint32_t id, uint32_t name);
    std::vector<std::tuple<uint32_t, uint32_t>>::iterator last;
};

class LinkAPIHint : public Hint
{
  public:
    ~LinkAPIHint() = default;

    void Activate() override;
};
class LinkWikiHint : public Hint
{
  public:
    ~LinkWikiHint() = default;
    void Activate() override;
    void Update(std::vector<std::tuple<uint32_t, uint32_t>> ids) override
    {
        Hint::Update(std::move(ids));
        last = incomplete.begin();
    }

  private:
    std::vector<std::tuple<uint32_t, uint32_t>>::iterator last;
};
} // namespace TWC

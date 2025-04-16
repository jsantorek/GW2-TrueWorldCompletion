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

    virtual void Update(std::vector<uint32_t> ids)
    {
        incompleteMapIds = std::move(ids);
    }

  protected:
    std::vector<uint32_t> incompleteMapIds;
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
    void Update(std::vector<uint32_t> ids) override
    {
        Hint::Update(std::move(ids));
        last = incompleteMapIds.begin();
    }

  private:
    std::vector<uint32_t>::iterator last;
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
    void Update(std::vector<uint32_t> ids) override
    {
        Hint::Update(std::move(ids));
        last = incompleteMapIds.begin();
    }

  private:
    std::vector<uint32_t>::iterator last;
};
} // namespace TWC

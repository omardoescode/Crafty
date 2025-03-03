
// Provide Settings for the UI
namespace ui {
class UIOptions {
public:
  typedef bool Option;

public:
  /**
   * @brief Constructor
   *
   * Initialize the various variables
   * TODO: Load Future Layout from a file
   */
  UIOptions();

  const Option& running() const;
  void close();

private:
  Option _running;
};
}  // namespace ui

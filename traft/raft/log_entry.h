#pragma once

#include <compare>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "traft/raft/identity.h"
#include "traft/utils/data.h"
#include "traft/utils/define.h"
#include "xxHash/xxh3.h"

namespace traft::raft {

class LogId {
 public:
  LogId() = default;
  LogId(int64_t term, int64_t index) : term_{term}, index_{index} {}
  LogId(LogId &&) = default;
  LogId &operator=(LogId &&) = default;

  class Hasher {
    std::uint64_t operator()(const LogId &id) const {
      return XXH3_64bits(id.GetData_(), sizeof(LogId));
    }
  };

  std::int64_t GetTerm() const { return term_; }
  std::int64_t GetIndex() const { return index_; }

  void SetTerm(int64_t term) { term_ = term; }
  void SetIndex(int64_t index) { index_ = index; }

  std::strong_ordering operator<=>(const LogId &rhs) const {
    if (term_ != rhs.term_) {
      return term_ <=> rhs.term_;
    }
    return index_ <=> rhs.index_;
  }

  std::string ToString() const;

 private:
  const char *GetData_() const { return reinterpret_cast<const char *>(this); }

  std::int64_t index_{kInvalidIndex};
  std::int64_t term_{kInvalidIndex};
};

class NoOpEntry {};

class QuitEntry {};

class DataEntry {
 public:
  DataEntry() = default;
  DataEntry(utils::Data date) : data_{std::move(date)} {}
  DataEntry(DataEntry &&) = default;
  DataEntry &operator=(DataEntry &&) = default;

 private:
  utils::Data data_;
};

class ConfigurationEntry {
 public:
  ConfigurationEntry() = default;
  ConfigurationEntry(ConfigurationEntry &&) = default;
  ConfigurationEntry &operator=(ConfigurationEntry &&) = default;

 private:
  std::vector<PeerId> peers_;
  std::vector<PeerId> old_peers_;
};

using Entry = std::variant<NoOpEntry, DataEntry, ConfigurationEntry, QuitEntry>;

class LogEntry {
 public:
  LogEntry() = default;
  LogEntry(utils::Data data) : entry_{DataEntry{std::move(data)}} {};
  LogEntry(LogEntry &&) = default;
  LogEntry &operator=(LogEntry &&) = default;

  void SetTerm(int64_t term) { id_.SetTerm(term); }
  void SetIndex(int64_t index) { id_.SetIndex(index); }

  std::int64_t GetTerm() const { return id_.GetTerm(); }
  std::int64_t GetIndex() const { return id_.GetIndex(); }
  Entry &GetEntry() { return entry_; }

 private:
  LogId id_{};
  Entry entry_{NoOpEntry{}};
};

}  // namespace traft::raft

#pragma once

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15
#define ENT_MAX_NETWORKED_ENTRY 16384

class CBaseHandle
{
public:
    CBaseHandle() noexcept :
        m_nIndex(INVALID_EHANDLE_INDEX) { }

    CBaseHandle(const int m_nEntry, const int m_nSerial) noexcept {
        m_nIndex = m_nEntry | (m_nSerial << NUM_SERIAL_NUM_SHIFT_BITS);
    }

    bool operator!=(const CBaseHandle& other) const noexcept {
        return m_nIndex != other.m_nIndex;
    }

    bool operator==(const CBaseHandle& other) const noexcept {
        return m_nIndex == other.m_nIndex;
    }

    bool operator<(const CBaseHandle& other) const noexcept {
        return m_nIndex < other.m_nIndex;
    }

    [[nodiscard]] bool IsValid() const noexcept {
        return m_nIndex != INVALID_EHANDLE_INDEX;
    }

    [[nodiscard]] int GetEntryIndex() const noexcept {
        return static_cast<int>(m_nIndex & ENT_ENTRY_MASK);
    }

    [[nodiscard]] int GetSerialNumber() const noexcept {
        return static_cast<int>(m_nIndex >> NUM_SERIAL_NUM_SHIFT_BITS);
    }

private:
    std::uint32_t m_nIndex;
};
